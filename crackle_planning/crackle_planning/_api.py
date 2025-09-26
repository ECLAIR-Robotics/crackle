
from crackle_planning.ros_interface import RosInterface

class PlannerAPI:
    def __init__(self, use_ros: bool, ros_interface: RosInterface = None): 
        if use_ros and ros_interface is None:
            raise ValueError("ros_interface must be provided if use_ros is True")
        else:
            print("Warning: PlannerAPI initialized without ROS interface. Some functionalities may be limited.")
            self.ros_interface = ros_interface
        self.use_ros = use_ros
        self.gripper_occupied = False
    
    def pick_up(self, object_name : str): # This function allows us to pick up object at called object_name
        if (self.gripper_occupied):
            print("Gripper is already holding an object.")
            return
        if self.use_ros:
            self.ros_interface.call_pickup_service(object_name)
        else:
            print(f"Simulating pick up of object '{object_name}' without ROS.")


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