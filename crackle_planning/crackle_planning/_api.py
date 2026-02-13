import os
import threading
import atexit
from typing import Dict, List

ROS_ENABLED = os.getenv("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.ros_interface import RosInterface
    import rclpy
    from rclpy.node import Node
    from rclpy.executors import MultiThreadedExecutor

# MOVE LATER
import numpy as np
import openai
from openai import OpenAI
from _keys import openai_key
os.environ["OPENAI_API_KEY"] = str(openai_key)
client = OpenAI(api_key=os.environ["OPENAI_API_KEY"])

class PlannerAPI:
    def __init__(self, use_ros: bool): 
        if use_ros:
            rclpy.init()
            self._crackle_node = Node("crackle_node")
            self._crackle_node.get_logger().info("Initialized PlannerAPI with ROS interface.")
            self.ros_interface = RosInterface(self._crackle_node)
            self.global_state: Dict[str, object] = {}
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

    # def find(self, object_name : str, mapped_objects): # This function is to locate the named object by matching it with the most similar mapped object

    # MOVE LATER
    def embedding_from_string(text: str, model: str = "text-embedding-3-small") -> np.ndarray:
        """Get an embedding as a NumPy vector."""
        resp = client.embeddings.create(model=model, input=text)
        # print("str: ", text, " | embedding: ", np.array(resp.data[0].embedding, dtype=np.float32))
        return np.array(resp.data[0].embedding, dtype=np.float32)

    # MOVE LATER
    def recommendations_from_strings(
        strings: List[str],
        index_of_source_string: int,
        model: str = "text-embedding-3-small",
    ) -> List[int]:
        """Return nearest neighbors of a given string (including itself first)."""

        # 1) embeddings for all strings
        embeddings = [embedding_from_string(s, model=model) for s in strings]

        # 2) source embedding
        query = embeddings[index_of_source_string]

        # 3) cosine *distance* = 1 - cosine similarity
        q_norm = np.linalg.norm(query)
        distances = []
        for e in embeddings:
            denom = q_norm * np.linalg.norm(e)
            sim = float(query @ e / denom) if denom != 0 else 0.0
            distances.append(1.0 - sim)

        # 4) nearest neighbors = smallest distance first
        indices_of_nearest_neighbors = list(np.argsort(distances))

        closest_idx = indices_of_nearest_neighbors[1]
        if distances[closest_idx] <= 0.5:
            return strings[closest_idx]
        return None

    def pick_up(self, object_name : str): # This function allows us to pick up object at called object_name
        if (self.gripper_occupied):
            print("Gripper is already holding an object.")
            return
        if self.use_ros:
            self.ros_interface.call
            self.ros_interface.call_pickup_service(object_name) 
        else:
            print(f"Simulating pick up of object '{object_name}' without ROS.")
    
    def look_at_sound_direction(self, wake_word_time: float):
        if self.use_ros:
            self.ros_interface.look_at_person(wake_word_time)
        else:
            print("Simulating look at sound direction without ROS.")

    def place(self): # This function allows us to place object down
        print("IT WORKS YAYAYAYA")
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
    
    def get_global_state_value(key: str):
        pass

    def recognize_person(self):
        names = self.ros_interface.recognize_person()
        print(f"Recognized persons: {names}")
