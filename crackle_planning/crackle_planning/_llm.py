# from crackle_planning.planner_lib._keys import key
import openai

from crackle_planning._keys import key
from crackle_planning._api import PlannerAPI
from crackle_planning.parse import parse_functions_to_json


class GptAPI:
    def __init__(self, key: str):
        self.api_key = key
        openai.api_key = key  # Set API key once in the constructor

    
    def get_command(self, prompt: str):
        #client = OpenAI()
        openai.api_key=key
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

      





