import os
import threading
import atexit
from typing import Any, Dict, List, Optional

# MOVE LATER
import numpy as np


def _create_openai_client():
    """Create an OpenAI client only when embedding utilities are used."""
    try:
        from openai import OpenAI
    except Exception as exc:
        raise RuntimeError(
            "OpenAI package is not available. Install 'openai' to use embedding helpers."
        ) from exc

    api_key = os.getenv("OPENAI_API_KEY")
    if not api_key:
        raise RuntimeError(
            "OPENAI_API_KEY is not set. Export it before using embedding helpers."
        )
    return OpenAI(api_key=api_key)

class PlannerAPI:
    def __init__(self, use_ros: bool, ros_node: Optional[Any] = None):
        self.use_ros = use_ros
        self.gripper_occupied = False
        self.global_state: Dict[str, Any] = {}
        self.ros_interface: Any = None

        self._crackle_node = None
        self._executor = None
        self._spin_thread = None
        self._owns_node = False
        self._owns_rclpy_context = False

        if use_ros:
            try:
                import rclpy
                from rclpy.executors import MultiThreadedExecutor
                from rclpy.node import Node
                from crackle_planning.ros_interface import RosInterface
            except Exception as exc:
                raise RuntimeError(
                    "ROS dependencies for PlannerAPI are unavailable. Ensure ROS 2 and "
                    "crackle_planning dependencies are sourced and installed."
                ) from exc

            if not rclpy.ok():
                rclpy.init()
                self._owns_rclpy_context = True

            if ros_node is not None:
                self._crackle_node = ros_node
            else:
                self._crackle_node = Node("crackle_node")
                self._owns_node = True

            self._crackle_node.get_logger().info("Initialized PlannerAPI with ROS interface.")
            self.ros_interface = RosInterface(self._crackle_node)

            if self._owns_node:
                self._executor = MultiThreadedExecutor()
                self._executor.add_node(self._crackle_node)
                self._spin_thread = threading.Thread(target=self._executor.spin, daemon=True)
                self._spin_thread.start()

            atexit.register(self._shutdown_ros)

        else:
            print("Warning: PlannerAPI initialized without ROS interface. Some functionalities may be limited.")
            self.ros_interface = None
            self._crackle_node = ros_node

    def _shutdown_ros(self):
        if not self.use_ros:
            return
        try:
            import rclpy

            if self._executor is not None and self._owns_node:
                self._executor.shutdown(timeout_sec=1.0)
            if self._crackle_node is not None and self._owns_node:
                self._crackle_node.destroy_node()
            if self._owns_rclpy_context:
                rclpy.try_shutdown()
        except Exception as e:
            print(f"[PlannerAPI] ROS shutdown warning: {e}") 

    # def find(self, object_name : str, mapped_objects): # This function is to locate the named object by matching it with the most similar mapped object

    # MOVE LATER
    def embedding_from_string(self, text: str, model: str = "text-embedding-3-small") -> np.ndarray:
        """Get an embedding as a NumPy vector."""
        client = _create_openai_client()
        resp = client.embeddings.create(model=model, input=text)
        # print("str: ", text, " | embedding: ", np.array(resp.data[0].embedding, dtype=np.float32))
        return np.array(resp.data[0].embedding, dtype=np.float32)

    # MOVE LATER
    def recommendations_from_strings(
        self,
        strings: List[str],
        index_of_source_string: int,
        model: str = "text-embedding-3-small",
    ) -> Optional[str]:
        """Return nearest neighbors of a given string (including itself first)."""

        # 1) embeddings for all strings
        embeddings = [self.embedding_from_string(s, model=model) for s in strings]

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

    def pick_up(self, object_name: str):
        if self.gripper_occupied:
            print("Gripper is already holding an object.")
            return
        if self.use_ros and self.ros_interface is not None:
            self.ros_interface.clear_and_refresh_octomap()
            self.ros_interface.call_pickup_service(object_name)
            self.gripper_occupied = True
        else:
            print(f"Simulating pick up of object '{object_name}' without ROS.")
    
    def look_at_sound_direction(self, wake_word_time: float):
        if self.use_ros and self.ros_interface is not None:
            self.ros_interface.look_at_person(wake_word_time)
        else:
            print("Simulating look at sound direction without ROS.")

    def place(self):
        if self.use_ros and self.ros_interface is not None:
            self.ros_interface.clear_and_refresh_octomap()
            self.gripper_occupied = False
        else:
            print("Simulating place without ROS.")

    def get_position_of(self, obj_name: str):
        pass

    def orient_gripper(self, roll: float, pitch: float, yaw: float):
        pass
    
    def wave(self):
        pass
    
    def set_emotion(self, emotion: str):
        if self.use_ros and self.ros_interface is not None:
            self.ros_interface.set_emotion(emotion)
        else:
            print(f"Simulating setting emotion to '{emotion}' without ROS.")
    
    def close_gripper(self):
        pass

    def open_gripper(self):
        pass

    def dance_dance(self):
        if self.use_ros and self.ros_interface is not None:
            self.ros_interface.dance()
        else:
            print("Simulating dance maneuver without ROS.")
    
    def get_global_state_value(self, key: str):
        return self.global_state.get(key)

    def recognize_person(self):
        if self.ros_interface is None:
            print("ROS interface unavailable; cannot recognize person.")
            return
        names = self.ros_interface.recognize_person()
        print(f"Recognized persons: {names}")
