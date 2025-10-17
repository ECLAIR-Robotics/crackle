# from crackle_planning.planner_lib._keys import key
import openai

from crackle_planning._keys import openai_key
from crackle_planning._api import PlannerAPI
from crackle_planning.parse import parse_functions_to_json


class GptAPI:
    def __init__(self, key: str | None):
        self.api_key = key or openai_key
        openai.api_key = self.api_key # Set API key once in the constructor


    def getTalkBack(self, prompt):
        promptPt1 = "you are a helpful (but slightly sarcastic) robot assistant having a conversation with a user. The user has just given you the following prompt: "
        promptPt2 = " Please return a response to this, including the intended emotion (chosen from 'happy', 'sad', 'angry', 'bored', 'not impressed', 'evil', 'flirty', 'aww', and 'wtf'')  as a word at the end of the response string"
        fullPrompt = promptPt1 + prompt + promptPt2
        description = []
        response = openai.chat.completions.create(
        model="gpt-4",
        messages=[
            {
                "role": "system",
                "content": "you are a helpful (but somewhat sarcastic and slightly offensive) robot assistant "
            },
            {"role": "user", "content": fullPrompt}
        ],
        functions=description,
        function_call="auto"
        )

        response_message = response.choices[0].message.content
        return response_message

    # calls getTalkBack, returns an array where item 0 is the response and item 1 is the emotion
    # recalls itself if item 1 is not a valid emotion
    def parseTalkResponse(self, prompt):
        response = self.getTalkBack(prompt)
        splitResponse = response.split()
        emotion = splitResponse[1]
        emotLen = len(emotion)
        fullLen = len(response)
        retLen = fullLen - emotLen
        responseWords = response[0: retLen]
        return [responseWords, emotion]


    
    def get_command(self, prompt: str):
        #client = OpenAI()
        # openai.api_key=opena
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

      





