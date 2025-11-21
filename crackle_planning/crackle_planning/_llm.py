# from crackle_planning.planner_lib._keys import key
from numpy import save
import openai
from openai import OpenAI

from _keys import openai_key
from _api import PlannerAPI
from parse import parse_functions_to_json
import os
from elevenlabs import ElevenLabs


class GptAPI:
    def __init__(self, key: str):
        self.api_key = key
        openai.api_key = openai_key  # Set API key once in the constructor
        self.client = OpenAI()
        print("ELEVENLABS_API_KEY:", os.getenv("ELEVENLABS_API_KEY"))
        self.tts = ElevenLabs(api_key=os.getenv("ELEVENLABS_API_KEY"))


    def getTalkBack(self, prompt):
        promptPt1 = "you are a helpful, strongly midwestern / minnesotan, almost Fargo-like, robot assistant with a strong obsession with movies / movie references having a conversation with a user. The user has just given you the following prompt: "
        promptPt2 = " Please return a response to this, including the intended emotion (chosen from 'happy', 'sad', 'angry', 'bored', 'not_impressed', 'evil', 'flirty', 'aww', and 'wtf'')  as a word at the end of the response string. Always have the emotion be after a keyword 'Emotion'"
        fullPrompt = promptPt1 + prompt + promptPt2
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
        # functions=description,
        # function_call="auto"
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
    
    def get_command(self, prompt: str):
        #client = OpenAI()
        openai.api_key=openai_key
        description=[]
        pickup={
            "name": "pick_up",
            "description": "Pick up the object for the user",
            "parameters": {
                "type": "object",
                "properties": {
                    "x": {
                        "type": "number",
                        "description": "The x coordinate of the object.",
                    },
                    "y": {
                        "type": "number",
                        "description": "The y coordinate of the object.",
                    },
                    "z": {
                        "type": "number",
                        "description": "The z coordinate of the object.",
                    }
                },
                "required": ["x", "y", "z"],
                "additionalProperties": False,
            },        
        }

        place={
            "name": "place",
            "description": "Place the object for the user",
            "parameters": {
                "type": "object",
                "properties": {
                    "x": {
                        "type": "number",
                        "description": "The x coordinate to put at.",
                    },
                    "y": {
                        "type": "number",
                        "description": "The y coordinate to put at.",
                    },
                    "z": {
                        "type": "number",
                        "description": "The z coordinate to put at.",
                    }
                },
                "required": ["x", "y", "z"],
                "additionalProperties": False,
            },        
        }
        description.append(pickup)
        description.append(place)
        prompt= "Pick up the object"
        response = openai.chat.completions.create(
        model="gpt-4",
        messages=[
            {
                "role": "system",
                "content": "You are a helpful robot assistant. Use the supplied functions to generate code that uses the provided list of functions"
            },
            {"role": "user", "content": prompt}
        ],
        functions=description,
        function_call="auto"
        )        
        action_name = response.choices[0].message.function_call
        print('Function to call:')
        print(action_name)
        return action_name
    
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

      





