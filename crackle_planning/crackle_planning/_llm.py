# from crackle_planning.planner_lib._keys import key
from numpy import save
import openai
from openai import OpenAI
import json
from crackle_planning._keys import openai_key
from crackle_planning._api import PlannerAPI
from crackle_planning.parse import parse_functions_to_json
import os
from elevenlabs import ElevenLabs

import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor

ROS_ENABLED = os.getenv("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.ros_interface import RosInterface

class GptAPI:
    def __init__(self, key: str):
        self.api_key = key
        openai.api_key = openai_key  # Set API key once in the constructor
        self.client = OpenAI()
        self.tts = ElevenLabs(api_key=os.getenv("ELEVENLABS_API_KEY"))


    def getTalkBack(self, prompt):
        
        # description = []
        response = openai.chat.completions.create(
        model="gpt-4",
        messages=[
            {
                "role": "system",
                "content": "you are a helpful, strongly midwestern / minnesotan, almost Fargo-like, robot assistant with a strong obsession with movies / movie references"
            },
            {"role": "user", "content": fullPrompt}
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


    def get_command(self, prompt: str, ros_interface: any = None):
        self.client = OpenAI()

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
                                "Executable Python code ONLY. No explanations or comments. "
                                "Use ONLY these existing methods:\n"
                                "  - def pick_up(self, object_name: str)\n"
                                "  - def place(self)\n"
                                "They belong to the api instance. So make sure to write code that " 
                                "calls these methods from api instance "
                                "Assume 'self' is already in scope so no need to pass it in. "
                                "Do not redefine these. For example, if you were to pick up and "
                                "place an object called 'x', code would look like this:\n"
                                "api.pick_up(\"x\")\\api.place()"
                            ),
                        },
                        "emotion": {
                            "type": "string",
                            "description": (
                                "One of: 'happy', 'sad', 'angry', 'bored', "
                                "'not_impressed', 'evil', 'flirty', 'aww', 'wtf'."
                            ),
                        },
                    },
                    "required": ["dialogue", "code", "emotion"],
                    "additionalProperties": False,
                },
                "strict": True,
            }
        ]

        instructions = (
            "Your name is Lleo. You are a helpful, strongly midwestern / minnesotan, "
            "almost Fargo-like robot assistant with a strong obsession with movies "
            "and movie references. You are great at breaking down tasks into small "
            "steps and interacting with the real world by generating Python code.\n\n"
            "For EVERY user prompt, you MUST call the tool 'make_response' exactly once.\n"
            "- Do NOT send any normal assistant text.\n"
            "- Put your conversational reply in the 'dialogue' field.\n"
            "- Put ONLY executable Python code in the 'code' field.\n"
            "- Put a single emotion word in the 'emotion' field.\n"
        )

        input_items = [
            {"role": "developer", "content": instructions},
            {"role": "user", "content": prompt},
        ]

        # Important: tool_choice is just the string "required" here.
        # Since we only define one custom tool, the model is forced to call it.
        response = self.client.responses.create(
            model="gpt-5-mini",
            reasoning={"effort": "minimal"},
            tools=tools,
            tool_choice="required",
            input=input_items,
        )

        dialogue_val = None
        code_val = None
        emotion_val = None

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

        # Debug prints if you want them
        if dialogue_val is not None:
            print("Dialogue:", dialogue_val)
        else:
            print("WARNING: No dialogue returned.")

        if code_val is not None:
            print("Code:", code_val)
        else:
            print("WARNING: No code returned.")

        if emotion_val is not None:
            print("Emotion:", emotion_val)
        else:
            print("WARNING: No emotion returned.")

        # Return all three so your planner can use them
        return {
            "dialogue": dialogue_val,
            "code": code_val,
            "emotion": emotion_val,
        }
    
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

    def speak_text_eleven_labs(self, text: str, output_path: str = "speech_output.mp3"):
        audio_stream = self.tts.text_to_speech.convert(
            voice_id="Tu7yBVBgg8rrFciOxBRm",
            model_id="eleven_multilingual_v2",
            text=text,
        )

        with open(output_path, "wb") as f:
            for chunk in audio_stream:
                if chunk:
                    f.write(chunk)

        return output_path

    
    def speech_to_text(self, speechFile, model="whisper-1"):
        """Converts speech to text using OpenAI's Whisper API"""
        with open(speechFile, "rb") as audio_file:
            transcript = self.client.audio.transcriptions.create(
                file=audio_file,
                model=model
            )
        return transcript.text
    
if __name__ == "__main__":
    gpt_api = GptAPI(openai_key)
    # return {
    #         "dialogue": dialogue_val,
    #         "code": code_val,
    #         "emotion": emotion_val,
    #     }
    output = gpt_api.get_command("Hey Leo you suck. You need to prove to me that you do not suck. Also, you're looking mighty sexy today. can you please bring me the bottle")
    gpt_api.speak_text_eleven_labs(output["dialogue"])
    # RosInterface.set_emotion(output["emotion"])
    api = PlannerAPI(ROS_ENABLED)
    api.set_emotion(output["emotion"]) 
    exec(output["code"])



