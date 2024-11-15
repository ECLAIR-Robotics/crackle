from crackle_planning.planner_lib._keys import api_key
from crackle_planning.planner_lib._api import PlannerAPI
from crackle_planning.planner_lib.parse import parse_functions_to_json


class GptAPI:
    def __init__(self, api_key: str):
        self.api_key = api_key
        self.api = PlannerAPI(api_key)

    def get_next_action(self):
        return self.api.L[0]
    
    def get_command():
        L = [PlannerAPI.pick_up, PlannerAPI.place, PlannerAPI.get_position_of, PlannerAPI.orient_gripper, PlannerAPI.wave]
        prompt= "Pick up the object"
        response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}],
        functions=parse_functions_to_json(),
        function_call="auto"
        #"tool_choice": "auto"
        )
        action=response.choices[0].message
        print(action)

print(api_key)        





