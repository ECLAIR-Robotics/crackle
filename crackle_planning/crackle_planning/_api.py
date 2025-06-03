
from crackle_planning.ros_interface import RosInterface

class PlannerAPI:
    def __init__(self, ros_interface: RosInterface): 
        self.ros_inteface = ros_interface
        self.gripper_occupied = False
    
    def pick_up(self, x : float, y : float, z: float): # This function allows us to pick up object at position x, y, z
        pass

    def place(self, x : float, y : float, z : float):
        pass

    def get_position_of(self, obj_name: str):
        pass

    def orient_gripper(self, roll: float, pitch: float, yaw: float):
        pass
    
    def wave(self):
        pass
    
    def set_emotion(self, emotion: str):
        pass
    
    def close_gripper(self):
        pass

    def open_gripper(self):
        pass