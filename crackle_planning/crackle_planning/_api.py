import os
import threading
import atexit

ROS_ENABLED = os.getenv("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.ros_interface import RosInterface
    import rclpy
    from rclpy.node import Node
    from rclpy.executors import MultiThreadedExecutor

class PlannerAPI:
    def __init__(self, use_ros: bool): 
        if use_ros:
            rclpy.init()
            self._crackle_node = Node("crackle_node")
            self._crackle_node.get_logger().info("Initialized PlannerAPI with ROS interface.")
            self.ros_interface = RosInterface(self._crackle_node)
            self._executor = MultiThreadedExecutor()
            self._executor.add_node(self._crackle_node)
            self._spin_thread = threading.Thread(target=self._executor.spin, daemon=True)
            self._spin_thread.start()
            atexit.register(self._shutdown_ros)

        else:
            print("Warning: PlannerAPI initialized without ROS interface. Some functionalities may be limited.")
            self.ros_interface = None
            self._crackle_node = None
            self._executor = None
        self.use_ros = use_ros
        self.gripper_occupied = False

    def _shutdown_ros(self):
        if not self.use_ros:
            return
        try:
            if self._executor is not None:
                self._executor.shutdown(timeout_sec=1.0)
            if self._crackle_node is not None:
                self._crackle_node.destroy_node()
            rclpy.try_shutdown()
        except Exception as e:
            print(f"[PlannerAPI] ROS shutdown warning: {e}") 

    def pick_up(self, object_name : str): # This function allows us to pick up object at called object_name
        if (self.gripper_occupied):
            print("Gripper is already holding an object.")
            return
        if self.use_ros:
            self.ros_interface.call_pickup_service(object_name) 
        else:
            print(f"Simulating pick up of object '{object_name}' without ROS.")
    
    def look_at_sound_direction(self, wake_word_time: float):
        if self.use_ros:
            self.ros_interface.look_at_person(wake_word_time)
        else:
            print("Simulating look at sound direction without ROS.")

    def place(self, x : float, y : float, z : float):
        pass

    def get_position_of(self, obj_name: str):
        pass

    def orient_gripper(self, roll: float, pitch: float, yaw: float):
        pass
    
    def wave(self):
        pass
    
    def set_emotion(self, emotion: str):
        if self.use_ros:
            self.ros_interface.set_emotion(emotion)
        else:
            print(f"Simulating setting emotion to '{emotion}' without ROS.")
    
    def close_gripper(self):
        pass

    def open_gripper(self):
        pass

    def dance_dance(self):
        if self.use_ros:
            self.ros_interface.dance()
        else:
            print("Simulating dance maneuver without ROS.")
