from openai import OpenAI
import json
import os
from typing import Optional
from elevenlabs import ElevenLabs
from dataclasses import dataclass

ROS_ENABLED = os.environ.get("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.parse import parse_class_to_tools
    from crackle_planning._keys import openai_key
    from crackle_planning._api import PlannerAPI
else:
    from parse import parse_class_to_tools
    from _keys import openai_key
    from _api import PlannerAPI

_SUBMIT_PLAN_TOOL = {
    "type": "function",
    "name": "submit_plan",
    "description": (
        "Before executing any robot actions, submit a step-by-step plan of every tool "
        "you will call and why. Think through prerequisites: if gripper_occupied=True "
        "you must place() before pick_up(). If no robot actions are needed, return an "
        "empty list. You will then execute the plan step by step."
    ),
    "parameters": {
        "type": "object",
        "properties": {
            "steps": {
                "type": "array",
                "items": {"type": "string"},
                "description": (
                    "Ordered list of robot action calls with brief reasoning, e.g. "
                    "['pick_up(water_bottle) — object requested by user', 'place() — deliver it']. "
                    "Empty list if only conversation is needed."
                ),
            }
        },
        "required": ["steps"],
        "additionalProperties": False,
    },
    "strict": True,
}

# PlannerAPI methods that are internal utilities, not robot actions the LLM should call.
_PLANNER_API_EXCLUDE = {
    "embedding_from_string",
    "recommendations_from_strings",
    "get_global_state_value",
    "look_at_sound_direction",
    "set_emotion",
}

@dataclass
class GetCommandResponse:
    dialoge: str | None
    emotion: str | None
    continue_talking: bool = False
    # Kept for backwards compatibility; always None in the new tool-calling flow.
    code: str | None = None


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
            "PLANNING: Every turn has two phases. First you will be asked to call "
            "submit_plan — think through the FULL sequence of actions upfront, including "
            "prerequisites and what to do if a step fails. Then you execute the plan one "
            "tool call at a time, reading each result before proceeding. If a step fails, "
            "adapt (try a recovery or skip). When all actions are done, call "
            "'make_final_response' with your conversational reply and emotion.\n\n"
            "GRIPPER STATE: The gripper can hold at most one object. You must call "
            "place() before picking up a new object. The gripper_occupied field in "
            "your context tells you the current state.\n\n"
            "SOCIAL INTERACTIONS: If the user greets you, compliments you, or just wants "
            "to chat, respond warmly and call wave() or dance_dance() as appropriate. "
            "Use movie references naturally in conversation.\n\n"
            "CLARIFICATION: Only set continue_talking=True if you genuinely cannot "
            "proceed — e.g., the user said 'pick that up' without naming an object. "
            "Never add a question just to justify continue_talking=True.\n\n"
            "RESPONSE STYLE: Short, warm, midwestern. Movie quotes when they fit."
        )

        # Persistent conversation history — survives across get_command calls.
        self.input_items = [
            {"role": "developer", "content": self._instructions},
        ]

    def _build_tools(self) -> list:
        """Build the full tool list: PlannerAPI action tools + make_final_response."""
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
                            "Default FALSE. Set True ONLY when you cannot proceed "
                            "without a clarifying answer from the user."
                        ),
                    },
                },
                "required": ["dialogue", "emotion", "continue_talking"],
                "additionalProperties": False,
            },
            "strict": True,
        }

        return action_tools + [final_response_tool]

    def _execute_tool(self, planner_api: "PlannerAPI", name: str, args: dict) -> dict:
        """Call a PlannerAPI method by name and return a structured result."""
        method = getattr(planner_api, name, None)
        if method is None:
            return {"success": False, "message": f"Unknown tool: {name}"}
        try:
            args.pop("self", None)
            result = method(**args)
            msg = f"{name} completed successfully."
            return {
                "success": True,
                "message": msg,
                "result": str(result) if result is not None else None,
            }
        except Exception as exc:
            return {"success": False, "message": f"{name} failed: {exc}"}

    def get_command(
        self,
        fsm_instance,
        prompt: str,
        planner_api: Optional["PlannerAPI"] = None,
    ) -> GetCommandResponse:
        """Run one conversational turn using an agentic tool-calling loop.

        The LLM can call PlannerAPI action tools zero or more times, then must
        call make_final_response to finish the turn.
        """
        if planner_api is None:
            planner_api = PlannerAPI(ROS_ENABLED)

        tools = self._build_tools()

        # Inject current robot state as context so the model can make informed decisions.
        state_context = (
            f"[Robot state] gripper_occupied={planner_api.gripper_occupied}"
        )
        user_content = f"{state_context}\n\nUser: {prompt}"

        user_msg = {"role": "user", "content": user_content}
        self.input_items.append(user_msg)
        fsm_instance.context_window.append({"role": "user", "content": prompt})

        # --- Phase 1: planning ---
        # Force the model to lay out the full action sequence before touching any tool.
        plan_response = self.client.responses.create(
            model="gpt-5-mini",
            reasoning={"effort": "medium"},
            tools=[_SUBMIT_PLAN_TOOL],
            tool_choice="required",
            input=self.input_items,
        )
        self.input_items.extend(plan_response.output)
        for item in plan_response.output:
            if item.type == "function_call" and item.name == "submit_plan":
                plan_steps = json.loads(item.arguments or "{}").get("steps", [])
                print(f"[plan] {plan_steps}")
                self.input_items.append({
                    "type": "function_call_output",
                    "call_id": item.call_id,
                    "output": json.dumps({"acknowledged": True, "steps": plan_steps}),
                })

        # --- Phase 2: action loop ---
        dialogue_val: Optional[str] = None
        emotion_val: str = "happy"
        continue_talking_val: bool = False

        max_iterations = 10
        for _ in range(max_iterations):
            response = self.client.responses.create(
                model="gpt-5-mini",
                reasoning={"effort": "medium"},
                tools=tools,
                tool_choice="required",
                input=self.input_items,
            )

            # Persist all output items (function_call, reasoning summaries, etc.)
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
                    # No tool result needed — this ends the loop.
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

        # Persist a compact assistant summary for the conversation window.
        assistant_msg = {
            "role": "assistant",
            "content": json.dumps({"dialogue": dialogue_val, "emotion": emotion_val}),
        }
        fsm_instance.context_window.append(assistant_msg)

        return GetCommandResponse(
            dialoge=str(dialogue_val) if dialogue_val is not None else None,
            emotion=str(emotion_val) if emotion_val is not None else None,
            continue_talking=bool(continue_talking_val),
        )

    def speak_text_eleven_labs(self, text: str):
        """Stream TTS audio directly to speakers via paplay — no file I/O, low latency."""
        import subprocess

        audio_stream = self.tts.text_to_speech.convert(
            voice_id="Tu7yBVBgg8rrFciOxBRm",
            model_id="eleven_turbo_v2_5",
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

    def speech_to_text(self, speechFile, model="whisper-1"):
        """Converts speech to text using OpenAI's Whisper API."""
        with open(speechFile, "rb") as audio_file:
            transcript = self.client.audio.transcriptions.create(
                file=audio_file,
                model=model
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
