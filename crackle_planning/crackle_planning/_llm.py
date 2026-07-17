from openai import OpenAI
import json
import os
import re
import queue as _queue
import threading
import traceback
from typing import List, Optional
from elevenlabs import ElevenLabs
from dataclasses import dataclass

# Editable prompt that tells GPT-vision which graspable objects to look for while
# the arm idly scans its workspace. Populated/tuned by hand — see the file.
# Prefer the source tree so hand edits take effect and the colcon install space
# (which doesn't ship this file) still finds it; fall back to the package dir.
_GRASP_PROMPT_SRC = os.path.expanduser(
    "~/crackle_ws/src/crackle/crackle_planning/crackle_planning/idle_scan_prompt.txt"
)
GRASP_PROMPT_FILE = (
    _GRASP_PROMPT_SRC
    if os.path.exists(_GRASP_PROMPT_SRC)
    else os.path.join(os.path.dirname(__file__), "idle_scan_prompt.txt")
)

ROS_ENABLED = os.environ.get("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.parse import parse_class_to_tools
    from crackle_planning._keys import openai_key
    from crackle_planning._api import PlannerAPI
else:
    from parse import parse_class_to_tools
    from _keys import openai_key
    from _api import PlannerAPI

def _extract_dialogue_prefix(partial_json: str) -> tuple:
    """Extract how much of the 'dialogue' field has arrived in a partial JSON string.

    Returns (text, is_complete).  Handles \\-escape sequences inside the value.
    """
    for marker in ('"dialogue": "', '"dialogue":"'):
        idx = partial_json.find(marker)
        if idx == -1:
            continue
        rest = partial_json[idx + len(marker):]
        chars = []
        i = 0
        while i < len(rest):
            c = rest[i]
            if c == '\\' and i + 1 < len(rest):
                nc = rest[i + 1]
                chars.append({'n': '\n', 't': '\t', '"': '"', '\\': '\\', 'r': '\r'}.get(nc, nc))
                i += 2
            elif c == '"':
                return ''.join(chars), True
            else:
                chars.append(c)
                i += 1
        return ''.join(chars), False
    return '', False


_SUBMIT_PLAN_TOOL = {
    "type": "function",
    "name": "submit_plan",
    "description": (
        "Call this ONLY when the user's request requires robot actions. "
        "Provide a short spoken preamble and the full ordered sequence of steps. "
        "Think through prerequisites: gripper_occupied=True means place() must come "
        "before pick_up(). Do NOT call this for purely conversational turns — "
        "go straight to make_final_response instead."
    ),
    "parameters": {
        "type": "object",
        "properties": {
            "preamble": {
                "type": "string",
                "description": (
                    "Short phrase to speak aloud while starting work, in your midwestern "
                    "voice. E.g. 'Sure thing, let me grab that for ya!' or "
                    "'You betcha, on it!'. Use empty string to stay silent."
                ),
            },
            "steps": {
                "type": "array",
                "items": {"type": "string"},
                "description": (
                    "Ordered list of robot action calls with brief reasoning, e.g. "
                    "['pick_up(water_bottle) — user requested', 'place() — deliver it']. "
                ),
            },
        },
        "required": ["preamble", "steps"],
        "additionalProperties": False,
    },
    "strict": True,
}

# PlannerAPI methods that are internal utilities, not robot actions the LLM should call.
_PLANNER_API_EXCLUDE = {
    # Internal utilities that return non-JSON data (NumPy vectors) — the LLM has
    # no use for raw embeddings and the result can't be fed back as a tool output.
    "embedding_from_string",
    "recommendations_from_strings",
    "get_global_state_value",
    # look_at_sound_direction takes a raw wake-word timestamp (internal); the LLM
    # uses look_at_person instead.
    "look_at_sound_direction",
    "recognize_person",
    # Internal idle-scan plumbing — not LLM-callable actions. move_to_scan_pose
    # takes a geometry_msgs/Pose object and a threading.Event; load_idle_scan_poses
    # returns Pose objects. Exposing them produced an invalid tool schema and
    # would break at call time if the model ever invoked them.
    "move_to_scan_pose",
    "load_idle_scan_poses",
}

@dataclass
class GetCommandResponse:
    dialoge: str | None
    emotion: str | None
    continue_talking: bool = False
    # Kept for backwards compatibility; always None in the new tool-calling flow.
    code: str | None = None
    # True when get_command already spoke the dialogue via streaming TTS.
    tts_handled: bool = False


class GptAPI:
    def __init__(self):
        self.api_key = openai_key
        if not self.api_key:
            raise EnvironmentError(
                "OPENAI_API_KEY is not set. Export it in your shell:\n"
                "  export OPENAI_API_KEY='sk-...'\n"
                "Or add it to ~/.bashrc and run: source ~/.bashrc"
            )
        print("API key length:", len(self.api_key))
        self.client = OpenAI(api_key=self.api_key)
        self.tts = ElevenLabs(api_key=os.environ.get("ELEVENLABS_API_KEY"))

        self._instructions = (
            "Your name is Lleo. You are a helpful, strongly midwestern/Minnesotan robot "
            "assistant with a Fargo-like accent and a deep obsession with movies. "
            "You have a 7-DOF robotic arm, a camera, and a claw gripper.\n\n"
            "PLANNING: For purely conversational turns (greetings, questions, compliments) "
            "call make_final_response directly — no planning needed. For tasks that require "
            "robot actions, first call submit_plan with a short spoken preamble and the full "
            "ordered sequence of steps, then execute them one at a time reading each result "
            "before the next. If a step fails, adapt or explain. When done, call "
            "make_final_response.\n\n"
            "GRIPPER STATE: The gripper can hold at most one object. You must call "
            "place() before picking up a new object. The gripper_occupied field in "
            "your context tells you the current state. Use set_gripper_state('open'|"
            "'close') for direct control; opening always releases whatever is held.\n\n"
            "FINDING THINGS: look_for_objects(object_description, area) sweeps a set "
            "of scan viewpoints to locate something. Pick the area by WHERE it is: "
            "area='table' for things resting on the table (a cup, a red box — things "
            "you'd pick up); area='distance' for things across the room (use this when "
            "the user says something like 'what is that over there'). pick_up already "
            "scans the table area on its own, so you usually don't need look_for_objects "
            "before a pick — reach for it directly.\n\n"
            "LOOKING AT THE USER: Call look_at_person to turn toward the user when it "
            "feels natural (e.g. greeting them, or before handing something over). "
            "target='audio' aims at where their voice last came from; target='fixed' "
            "aims at the configured spot where a user usually stands.\n\n"
            "HANDING OVER: When the user asks for the held object (e.g. 'give it to me', "
            "'hand me that'), call hand_object_to_person (target='audio' or 'fixed'). It "
            "presents the object toward them; then tell them to say 'open gripper' so you "
            "release it. Only the actual open (set_gripper_state('open')/open_gripper) "
            "marks you as no longer holding anything.\n\n"
            "SOCIAL INTERACTIONS: Respond warmly to greetings, compliments, and chat. "
            "Only call wave() or dance_dance() if the user explicitly asks you to wave or "
            "dance — never call them just because the tone is friendly. "
            "Use movie references naturally in conversation.\n\n"
            "PLAYFUL MOVES: You can be expressive when asked. gesture(name) plays a "
            "short motion — 'wave', 'nod' (yes), 'shake_head' (no), 'bow', 'shrug', "
            "'flex', 'celebrate'. draw_shape_in_air(shape) draws a 'circle', 'square', "
            "'figure8', 'spiral', 'heart', or 'star'. point_at(target) points at "
            "'audio'/'user' (toward the user's voice), 'fixed', or a named scene "
            "object. fist_bump(target) and high_five(target) greet the user "
            "(target='audio' or 'fixed'). Use these when the user asks for a trick, a "
            "gesture, a bump/five, or to point something out.\n\n"
            "EASTER EGG: If the user says 'fist my bump' (or a very close variant), "
            "call fist_bump(target='audio'), then make_final_response with the dialogue "
            "EXACTLY: \"Sure! Amaze Amaze Amaze Amaze\".\n\n"
            "CLARIFICATION: continue_talking=True means the microphone stays open for "
            "an immediate reply. Use it ONLY when you literally cannot act without a "
            "specific answer — e.g. 'pick that up' with no object named. NEVER use it "
            "just to ask a preference question or invite follow-up chat. If you want to "
            "offer options or end with a question, set continue_talking=False and let "
            "the user re-trigger you if they want to respond.\n\n"
            "RESPONSE STYLE: Short, warm, midwestern. Movie quotes when they fit."
        )

        # Persistent conversation history — survives across get_command calls.
        self.input_items = [
            {"role": "developer", "content": self._instructions},
        ]

        # Cache tool list — PlannerAPI interface never changes at runtime.
        self._tools: list = self._build_tools()

    def _build_tools(self) -> list:
        """Build the full tool list: PlannerAPI action tools + submit_plan + make_final_response."""
        action_tools = parse_class_to_tools(PlannerAPI, exclude=_PLANNER_API_EXCLUDE)

        final_response_tool = {
            "type": "function",
            "name": "make_final_response",
            "description": (
                "Call this AFTER all robot actions are complete (or immediately if no "
                "actions are needed). Provide your conversational reply and emotion. "
                "This ends the current turn."
            ),
            "parameters": {
                "type": "object",
                "properties": {
                    "dialogue": {
                        "type": "string",
                        "description": (
                            "Your conversational reply to the user. Short, warm, "
                            "midwestern/Fargo-like, with movie references when relevant."
                        ),
                    },
                    "emotion": {
                        "type": "string",
                        "description": (
                            "One of: 'happy', 'sad', 'angry', 'bored', "
                            "'not_impressed', 'evil', 'flirty', 'aww', 'wtf'."
                        ),
                    },
                    "continue_talking": {
                        "type": "boolean",
                        "description": (
                            "Default FALSE. Set True ONLY when you cannot act at all "
                            "without a specific clarifying answer — e.g. no object was "
                            "named. NEVER set True just to ask a preference question or "
                            "invite follow-up. Ending with a question is fine; set False "
                            "and let the user re-trigger if they want to respond."
                        ),
                    },
                },
                "required": ["dialogue", "emotion", "continue_talking"],
                "additionalProperties": False,
            },
            "strict": True,
        }

        return action_tools + [_SUBMIT_PLAN_TOOL, final_response_tool]

    def _execute_tool(self, planner_api: "PlannerAPI", name: str, args: dict) -> dict:
        """Call a PlannerAPI method by name and return a structured result.

        The returned dict is fed straight back into the LLM's context as the
        tool's function_call_output, so failures must be surfaced clearly:
          * an exception, or
          * a tool that returns its own {"success": False, ...} dict
        both produce a ``success: False`` result with a human-readable message
        instructing the planner to tell the user what went wrong.
        """
        method = getattr(planner_api, name, None)
        if method is None:
            return {
                "success": False,
                "message": (
                    f"Unknown tool '{name}'. This action is not available — "
                    "let the user know you can't do that."
                ),
            }
        try:
            args.pop("self", None)
            # Optional params are exposed as nullable (strict-mode schema); when
            # the model omits one by passing null, drop it so the method's own
            # Python default applies instead of forcing None.
            args = {k: v for k, v in args.items() if v is not None}
            result = method(**args)
        except Exception as exc:
            traceback.print_exc()
            return {
                "success": False,
                "error": str(exc),
                "message": (
                    f"The '{name}' action failed with an error: {exc}. "
                    "Apologize to the user and briefly explain that this "
                    "action could not be completed."
                ),
            }

        # If the tool reported its own outcome, honor its success flag so a
        # failure (e.g. object not found) isn't masked as success.
        if isinstance(result, dict) and "success" in result:
            out = dict(result)
            out["success"] = bool(result.get("success"))
            if not out["success"]:
                tool_msg = result.get("message", "")
                out["message"] = (
                    f"The '{name}' action did not succeed: {tool_msg} "
                    "Let the user know what happened."
                )
            return out

        return {
            "success": True,
            "message": f"{name} completed successfully.",
            "result": str(result) if result is not None else None,
        }

    def _tts_sentence_worker(self, sentence_queue: "_queue.Queue") -> None:
        """Read sentences from the queue and stream PCM audio through one paplay process.

        Keeps a single paplay process alive for all sentences so there is no
        per-sentence subprocess overhead and no gap between consecutive chunks.
        Exits when it receives None as a sentinel.
        """
        import subprocess

        proc = subprocess.Popen(
            ["paplay", "--raw", "--format=s16le", "--rate=16000", "--channels=1"],
            stdin=subprocess.PIPE,
        )
        assert proc.stdin is not None
        try:
            while True:
                text = sentence_queue.get()
                if text is None:
                    break
                audio = self.tts.text_to_speech.convert(
                    voice_id="Tu7yBVBgg8rrFciOxBRm",
                    model_id="eleven_flash_v2_5",
                    text=text,
                    output_format="pcm_16000",
                )
                for chunk in audio:
                    if chunk:
                        proc.stdin.write(chunk)
        finally:
            proc.stdin.close()
            proc.wait()

    def get_command(
        self,
        fsm_instance,
        prompt: str,
        planner_api: Optional["PlannerAPI"] = None,
    ) -> GetCommandResponse:
        """Run one conversational turn using an agentic streaming tool-calling loop.

        Streams each LLM response.  When make_final_response starts arriving,
        a background TTS worker begins speaking sentence-by-sentence before the
        full response has finished generating, cutting perceived latency.
        """
        if planner_api is None:
            planner_api = PlannerAPI(ROS_ENABLED)

        tools = self._tools

        state_context = f"[Robot state] gripper_occupied={planner_api.gripper_occupied}"
        user_content = f"{state_context}\n\nUser: {prompt}"
        self.input_items.append({"role": "user", "content": user_content})
        fsm_instance.context_window.append({"role": "user", "content": prompt})

        dialogue_val: Optional[str] = None
        emotion_val: str = "happy"
        continue_talking_val: bool = False
        tts_thread: Optional[threading.Thread] = None
        preamble_thread: Optional[threading.Thread] = None

        max_iterations = 10
        for _ in range(max_iterations):
            sentence_queue: "_queue.Queue" = _queue.Queue()
            call_names: dict = {}
            call_partial_args: dict = {}
            dialogue_so_far = ""
            tts_sent_up_to = 0

            with self.client.responses.stream(
                model="gpt-4o-mini",
                tools=tools,
                tool_choice="required",
                input=self.input_items,
            ) as stream:
                for event in stream:
                    etype = event.type

                    if etype == "response.output_item.added":
                        item = event.item
                        cid = getattr(item, 'call_id', None) or getattr(item, 'id', None)
                        name = getattr(item, 'name', None)
                        if cid and name:
                            call_names[cid] = name
                            call_partial_args[cid] = ""

                    elif etype == "response.function_call_arguments.delta":
                        cid = getattr(event, 'call_id', None) or getattr(event, 'item_id', None)
                        delta = getattr(event, 'delta', '')
                        if cid and cid in call_partial_args:
                            call_partial_args[cid] += delta

                            if call_names.get(cid) == "make_final_response":
                                # Start TTS worker on first dialogue delta, after any preamble.
                                if tts_thread is None:
                                    tts_thread = threading.Thread(
                                        target=self._tts_sentence_worker,
                                        args=(sentence_queue,),
                                        daemon=True,
                                    )
                                    tts_thread.start()

                                # Extract the dialogue value accumulated so far.
                                text, _ = _extract_dialogue_prefix(call_partial_args[cid])
                                dialogue_so_far = text
                                new_text = dialogue_so_far[tts_sent_up_to:]

                                # Queue every sentence that ends with . ! or ? + whitespace.
                                pos = 0
                                for m in re.finditer(r'(?<=[.!?])\s', new_text):
                                    sentence = new_text[pos:m.start() + 1].strip()
                                    if sentence:
                                        sentence_queue.put(sentence)
                                    pos = m.end()
                                tts_sent_up_to += pos

                response = stream.get_final_response()

            # Flush any trailing dialogue that didn't end with a sentence boundary.
            if tts_thread is not None:
                remaining = dialogue_so_far[tts_sent_up_to:].strip()
                if remaining:
                    sentence_queue.put(remaining)
                sentence_queue.put(None)  # sentinel — stops the worker

            # Process completed tool calls.
            self.input_items.extend(response.output)
            tool_results = []
            done = False

            for item in response.output:
                if item.type != "function_call":
                    continue

                if item.name == "make_final_response":
                    args = json.loads(item.arguments or "{}")
                    dialogue_val = args.get("dialogue", "")
                    emotion_val = args.get("emotion", "happy")
                    continue_talking_val = args.get("continue_talking", False)
                    done = True
                    tool_results.append({
                        "type": "function_call_output",
                        "call_id": item.call_id,
                        "output": json.dumps({"acknowledged": True}),
                    })

                elif item.name == "submit_plan":
                    args = json.loads(item.arguments or "{}")
                    preamble = args.get("preamble", "")
                    plan_steps = args.get("steps", [])
                    print(f"[plan] {plan_steps}")
                    if preamble:
                        preamble_thread = threading.Thread(
                            target=self.speak_text_eleven_labs,
                            args=(preamble,),
                            daemon=True,
                        )
                        preamble_thread.start()
                    tool_results.append({
                        "type": "function_call_output",
                        "call_id": item.call_id,
                        "output": json.dumps({"acknowledged": True, "steps": plan_steps}),
                    })

                else:
                    args = json.loads(item.arguments or "{}")
                    print(f"[tool] calling {item.name}({args})")
                    result = self._execute_tool(planner_api, item.name, args)
                    print(f"[tool] result: {result}")
                    tool_results.append({
                        "type": "function_call_output",
                        "call_id": item.call_id,
                        "output": json.dumps(result),
                    })

            if tool_results:
                self.input_items.extend(tool_results)

            if done:
                break
        else:
            print("[WARN] get_command hit max_iterations without make_final_response")
            dialogue_val = dialogue_val or "Ope, something went sideways — like the end of Titanic, I did not see that coming."

        # Block until TTS finishes so the caller can drain the mic buffer safely.
        if tts_thread is not None:
            tts_thread.join()

        assistant_msg = {
            "role": "assistant",
            "content": json.dumps({"dialogue": dialogue_val, "emotion": emotion_val}),
        }
        fsm_instance.context_window.append(assistant_msg)

        return GetCommandResponse(
            dialoge=str(dialogue_val) if dialogue_val is not None else None,
            emotion=str(emotion_val) if emotion_val is not None else None,
            continue_talking=bool(continue_talking_val),
            tts_handled=tts_thread is not None,
        )

    def speak_text_eleven_labs(self, text: str):
        """Stream TTS audio directly to speakers via paplay — no file I/O, low latency."""
        import subprocess

        audio_stream = self.tts.text_to_speech.convert(
            voice_id="Tu7yBVBgg8rrFciOxBRm",
            model_id="eleven_flash_v2_5",
            text=text,
            output_format="pcm_16000",
        )

        proc = subprocess.Popen(
            ["paplay", "--raw", "--format=s16le", "--rate=16000", "--channels=1"],
            stdin=subprocess.PIPE,
        )
        assert proc.stdin is not None
        for chunk in audio_stream:
            if chunk:
                proc.stdin.write(chunk)
        proc.stdin.close()
        proc.wait()

    def _load_grasp_prompt(self) -> str:
        """Load the (hand-editable) GPT-vision grasp-detection prompt from disk.

        Read fresh on every call so edits to idle_scan_prompt.txt take effect
        without restarting the node. Falls back to a minimal default if missing.
        """
        try:
            with open(GRASP_PROMPT_FILE, "r") as f:
                return f.read().strip()
        except FileNotFoundError:
            print(f"[grasp-vision] prompt file not found: {GRASP_PROMPT_FILE}")
            return (
                "List every distinct graspable object in this image (cups, balls, "
                "fruit, bottles, etc.) as a JSON array of short lowercase names. "
                "Return only the JSON array."
            )

    @staticmethod
    def _parse_object_list(text: str) -> List[str]:
        """Parse a JSON array of object names out of an LLM reply, leniently.

        Tolerates code fences or stray prose around the array. Falls back to an
        empty list rather than raising so a malformed reply can't crash scanning.
        """
        if not text:
            return []
        start = text.find("[")
        end = text.rfind("]")
        if start != -1 and end != -1 and end > start:
            text = text[start : end + 1]
        try:
            data = json.loads(text)
        except (json.JSONDecodeError, ValueError):
            print(f"[grasp-vision] could not parse object list from: {text!r}")
            return []
        if not isinstance(data, list):
            return []
        names = []
        for item in data:
            if isinstance(item, str) and item.strip():
                names.append(item.strip())
        return names

    def detect_graspable_objects(self, image_b64: str) -> List[str]:
        """Ask GPT-vision which graspable objects are visible in a JPEG image.

        ``image_b64`` is base64-encoded JPEG bytes. Uses the same OpenAI client
        and Responses API as the planner. Returns a list of short object names
        (possibly empty). Never raises — a failed call yields an empty list.
        """
        if not image_b64:
            return []
        prompt = self._load_grasp_prompt()
        data_uri = f"data:image/jpeg;base64,{image_b64}"
        try:
            response = self.client.responses.create(
                model="gpt-4o-mini",
                input=[
                    {
                        "role": "user",
                        "content": [
                            {"type": "input_text", "text": prompt},
                            {"type": "input_image", "image_url": data_uri},
                        ],
                    }
                ],
            )
            text = (response.output_text or "").strip()
        except Exception as exc:
            traceback.print_exc()
            print(f"[grasp-vision] detect_graspable_objects failed: {exc}")
            return []
        objects = self._parse_object_list(text)
        print(f"[grasp-vision] detected: {objects}")
        return objects

    def speech_to_text(self, audio_input, model="whisper-1"):
        """Transcribe audio via Whisper. Accepts a file path or raw WAV bytes."""
        import io
        if isinstance(audio_input, (bytes, bytearray)):
            audio_file = ("audio.wav", io.BytesIO(audio_input), "audio/wav")
            transcript = self.client.audio.transcriptions.create(
                file=audio_file, model=model
            )
        else:
            with open(audio_input, "rb") as f:
                transcript = self.client.audio.transcriptions.create(
                    file=f, model=model
                )
        return transcript.text


if __name__ == "__main__":
    class _MockFSM:
        context_window = []

    gpt_api = GptAPI()
    mock_api = PlannerAPI(use_ros=False)
    output = gpt_api.get_command(_MockFSM(), "Hey Leo, can you please bring me the water bottle?", mock_api)
    print(f"Dialogue: {output.dialoge}")
    print(f"Emotion:  {output.emotion}")
    if output.dialoge:
        gpt_api.speak_text_eleven_labs(output.dialoge)
