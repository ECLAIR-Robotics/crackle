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
        promptPt1 = "you are a helpful, strongly midwestern / minnesotan, almost Fargo-like, robot assistant with a strong obsession with movies / movie references having a conversation with a user. Keep the responses short and concise. Don't ramble. The user has just given you the following prompt: "
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
            "description": "Pick up the object for the user. Accepts a string (name of the object that has to be picked)",
            "parameters": {
                "type": "object",
                "properties": {
                    "object_name": {
                        "type": "string",
                        "description": "The object that has to be picked up.",
                    },
                    "example_call": "func.pick_up(phone)",
                },
                "required": ["object_name"],
                "additionalProperties": False,
            },        
        }

        place={
            "name": "place",
            "description": "Place the object for the user. Accepts a string (name of the object that has to be placed)",
            "parameters": False,
            "additionalProperties": False,
        }       
        
        pickup_string = json.dumps(pickup)
        place_string = json.dumps(place)
        description.append(pickup)
        description.append(place)

        gpt_content = "You are a helpful robot assistant. " \
        "You are great at breaking down a task into small functionalities based on the provided to you. " \
        "Then, generate code in python that calls the appropriate functions in the correct order that are required to execute the user's prompt. " \
        "Generate only code do not write anything else. Your output must be working, executable python code. You have the following functions. They belong to the func instance. So make sure to write code that calls these methods from func instance " \
        "The method descriptions and the parameters they accept are detailed here: " 

        response = self.client.responses.create(
            model="gpt-5",
            reasoning={"effort": "low"},
            input=[
                {
                    "role": "developer",
                    "content": gpt_content + pickup_string + place_string
                },
                {
                    "role": "user",
                    "content": prompt
                }
            ]
        )    
        func = PlannerAPI(ROS_ENABLED)

        print(response.output_text)
        exec(response.output_text)
        #print('Function to call:')
        #print(action_name)
        return response.output_text
    
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
            voice_id="Yut1fkzlqYCvxq6ubmvn",
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
    gpt_api.get_command("Bring me the bottle")





