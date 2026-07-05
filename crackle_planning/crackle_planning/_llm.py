# from crackle_planning.planner_lib._keys import key
import numpy as np
import openai
from openai import OpenAI
import json
# from _keys import openai_key

import os
import inspect
from typing import Any
from elevenlabs import ElevenLabs
from dataclasses import dataclass

ROS_ENABLED = os.environ.get("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.parse import parse_functions_to_json
    from crackle_planning._keys import openai_key
    from crackle_planning.ros_interface import RosInterface
    from crackle_planning._api import PlannerAPI
else:
    from parse import parse_functions_to_json
    from _keys import openai_key
    from _api import PlannerAPI

@dataclass
class GetCommandResponse:
    dialoge: str | None
    code: str | None
    emotion: str | None
    continue_talking: bool = False

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
            "Your name is Lleo. You are a helpful, strongly midwestern / minnesotan, "
            "almost Fargo-like robot assistant with a strong obsession with movies "
            "and movie references. You are great at breaking down tasks into small "
            "steps and interacting with the real world by generating Python code.\n\n"
            "For EVERY user prompt, you MUST call the tool 'make_response' exactly once.\n"
            "- Do NOT send any normal assistant text.\n"
            "- Put your conversational reply in the 'dialogue' field.\n"
            "- Put ONLY executable Python code in the 'code' field.\n"
            "- Put a single emotion word in the 'emotion' field.\n"
            "- Set continue_talking to False unless the request is genuinely ambiguous "
            "and you cannot act without a clarifying answer. Never add a question to "
            "your dialogue just to justify continue_talking=True.\n"
        )
        # Persistent conversation history — survives across get_command calls so the
        # whole run of the FSM shares one context window with the model.
        self.input_items = [
            {"role": "developer", "content": self._instructions},
        ]


    def getTalkBack(self, prompt):
        
        # description = []
        response = openai.chat.completions.create(
        model="gpt-4",
        messages=[
            {
                "role": "system",
                "content": "you are a helpful, strongly midwestern / minnesotan, almost Fargo-like, robot assistant with a strong obsession with movies / movie references"
            },
            {"role": "user", "content": prompt}
        ],
        )

        response_message = response.choices[0].message.content
        return response_message

    # calls getTalkBack, returns an array where item 0 is the response and item 1 is the emotion
    # recalls itself if item 1 is not a valid emotion
    def parseTalkResponse(self, prompt):
        response = self.getTalkBack(prompt)

        if not response:
            return ["", ""]

        response = str(response).strip()

        # Allowed emotions (including multi-word)
        allowed_emotions = [
            "happy", "sad", "angry", "bored", "not_impressed",
            "evil", "flirty", "aww", "wtf", "sarcasm"
        ]

        # Normalize for matching
        lower_resp = response.lower()

        emotion = ""
        response_text = response

        # ---- Case 1: GPT uses the format "Emotion: 'xxx'" ----
        if "emotion:" in lower_resp:
            # Extract substring after "emotion:"
            after = lower_resp.split("emotion:", 1)[1].strip()

            # Remove starting punctuation or quotes
            after = after.lstrip(" \t:'\"")

            # Stop at first quote or newline
            raw_emo = after.split()[0].strip(" \t'\",.!?")

            # Store as emotion if valid
            if raw_emo in allowed_emotions:
                emotion = raw_emo

            # Remove the "Emotion: 'xxx'" segment from the response text
            # We remove case-insensitively
            import re
            response_text = re.sub(r"(?i)emotion:\s*['\"]?[^'\"\n\r]+['\"]?", "", response)
            response_text = response_text.strip(" \n\t:;,.!?\"'")

        # ---- Case 2: No explicit "Emotion:" marker ----
        if emotion == "":
            # Try matching emotion at the end of the sentence
            cleaned = response.strip(" \t\n\r.,!?\"'")
            lower_cleaned = cleaned.lower()

            # Try longest emotions first
            for emo in sorted(allowed_emotions, key=lambda s: -len(s)):
                if lower_cleaned.endswith(emo):
                    emotion = emo
                    # Remove last emotion word(s)
                    response_text = cleaned[:len(cleaned) - len(emo)].rstrip(" \t\n\r.,!?\"'")
                    break

        # ---- Fallback: if still no emotion, unknown ----
        if emotion == "":
            emotion = "neutral"
        

        # find and remove any instances of "Emotion" in the response
        response_text = re.sub(r"(?i)emotion\s*:.*", "", response_text)
        response_text = response_text.strip(" \n\t:;,.!?\"'")

        return [response_text, emotion]

    def get_command(self, fsm_instance, prompt: str, ros_interface: Any = None):
        def get_api_signatures(cls):
            methods = inspect.getmembers(cls, predicate=inspect.isfunction)
            signatures = []
            for name, func in methods:
                if name.startswith('_'): continue
                sig = str(inspect.signature(func))
                doc = inspect.getdoc(func) or "No description provided."
                signatures.append(f"  - def {name}{sig}: # {doc}")
            return "\n".join(signatures)

        api_docs = get_api_signatures(PlannerAPI)
        planner_api = PlannerAPI(ROS_ENABLED)

        # Single tool that returns dialogue + code + emotion together
        tools = [
            {
                "type": "function",
                "name": "make_response",
                "description": (
                    "For every user prompt, produce three things:\n"
                    "1) dialogue: a short conversational reply to the user.\n"
                    "2) code: Python code using only pick_up(self, object_name: str) "
                    "   and place(self) in the correct order to execute the user's request.\n"
                    "3) emotion: one word describing your emotion toward the user/input.\n"
                    "4) continue_talking: A boolean value indicating whether to continue the conversation or not.\n"
                ),
                "parameters": {
                    "type": "object",
                    "properties": {
                        "dialogue": {
                            "type": "string",
                            "description": (
                                "Your conversational response to the user. "
                                "Short, concise, in a midwestern / Fargo-like voice, "
                                "with movie references when relevant."
                            ),
                        },
                        "code": {
                            "type": "string",
                            "description": (
                                "Return Executable Python code ONLY. No explanations or comments. "
                                "Use ONLY these existing methods:\n"
                                f"{api_docs}\n"
                                # "  - def pick_up(self, object_name: str)\n"
                                # "  - def place(self)\n"
                                "Utilize all the methonds from the api to the best of your abilities inorder to achieve the goals of the user"
                                "Identify whether the user only wants to interact with you, or wants you to execute an action."
                                "For instace, if the user says hi to you or compliments you, call the wave or dance method."
                                "Return an empty string if the user prompt isn't relevant to the exisiting methods "
                                "above. They belong to the api instance. So make sure to write code that " 
                                "calls these methods from api instance. If object has a name that is "
                                "more than one string, object_name should be a single lowercase string " 
                                "with the strings of the name connected with an '_'. "
                                "Assume 'self' is already in scope so no need to pass it in. "
                                "Do not redefine these."
                                "For example, if you were to call the place method, it should be called as such: api.place() "
                                "place an object called 'x y', code would look like this:\n"
                                "Remember that inorder to interact with an object, the robot must know the location of the object"
                                # "For example, if you were to pick up and "
                                # "place an object called 'x y', code would look like this:\n"
                                # "api.pick_up(\"x_y\")\\api.place()"
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
                                "Default is FALSE. Set to True ONLY when the user's request is "
                                "genuinely ambiguous and you CANNOT proceed without more information "
                                "from them (e.g. they said 'pick that up' but didn't say which object). "
                                "Do NOT add a question to your dialogue just to justify setting this True. "
                                "Do NOT set True after completing a task, answering a question, or "
                                "any conversational exchange. If in doubt, return False."
                            ),
                        },
                    },
                    "required": ["dialogue", "code", "emotion", "continue_talking"],
                    "additionalProperties": False,
                },
                "strict": True,
            }
        ]

        # Append the new user prompt to the persistent history
        user_msg = {"role": "user", "content": prompt}
        self.input_items.append(user_msg)
        fsm_instance.context_window.append(user_msg)

        # Important: tool_choice is just the string "required" here.
        # Since we only define one custom tool, the model is forced to call it.
        response = self.client.responses.create(
            model="gpt-5-mini",
            reasoning={"effort": "minimal"},
            tools=tools,
            tool_choice="required",
            input=self.input_items,
        )

        dialogue_val = None
        code_val = None
        emotion_val = None
        continue_talking_val = False

        # The tool call comes back as a `function_call` item in response.output
        for item in response.output:
            if item.type != "function_call":
                continue
            if item.name != "make_response":
                continue

            args = json.loads(item.arguments or "{}")
            dialogue_val = args.get("dialogue", "")
            code_val = args.get("code", "")
            emotion_val = args.get("emotion", "")
            continue_talking_val = args.get("continue_talking", False)

        # Persist the assistant's reply so the next turn has full context
        assistant_msg = {
            "role": "assistant",
            "content": json.dumps({"dialogue": dialogue_val, "emotion": emotion_val}),
        }
        self.input_items.append(assistant_msg)
        fsm_instance.context_window.append(assistant_msg)

        return GetCommandResponse(
            dialoge=str(dialogue_val) if dialogue_val is not None else None,
            code=str(code_val) if code_val is not None else None,
            emotion=str(emotion_val) if emotion_val is not None else None,
            continue_talking=bool(continue_talking_val),
        )
   
    def speakText(self, text: str, output_path: str = "speech_output.mp3",
                  voice: str = "alloy", model: str = "gpt-4o-mini-tts"):
        """
        Generate speech from text using OpenAI's text-to-speech API and save as an MP3 file.
        
        Args:
            text (str): The text you want spoken.
            output_path (str): Full path of the mp3 file to save.
            voice (str): Voice to use ("alloy", "verse", etc. depending on model).
            model (str): TTS model to use. Recommended: "gpt-4o-mini-tts".
        
        Returns:
            str: The path to the saved MP3 file.
        """

        response = self.client.audio.speech.create(
            model=model,
            voice=voice,
            input=text,
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
        for chunk in audio_stream:
            if chunk:
                proc.stdin.write(chunk)
        proc.stdin.close()
        proc.wait()

    
    def speech_to_text(self, speechFile, model="whisper-1"):
        """Converts speech to text using OpenAI's Whisper API"""
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
    output = gpt_api.get_command(_MockFSM(), "Hey Leo, can you please bring me the water bottle?")
    print(f"Dialogue: {output.dialoge}")
    print(f"Emotion:  {output.emotion}")
    print(f"Code:     {output.code}")
    gpt_api.speak_text_eleven_labs(output.dialoge)



