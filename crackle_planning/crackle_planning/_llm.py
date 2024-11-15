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
        L = [PlannerAPI.pick_up, PlannerAPI.place, PlannerAPI.get_position_of, PlannerAPI.orient_gripper, PlannerAPI.wave]
        description=parse_functions_to_json(L)
        #prompt= "Pick up the object"
        response = openai.chat.completions.create(
        model="gpt-4",
        messages=[{"role": "user", "content": prompt}],
        functions=description,
        function_call="auto"
        #"tool_choice": "auto"
        )
        action=response.choices[0].text        
        return action

      





