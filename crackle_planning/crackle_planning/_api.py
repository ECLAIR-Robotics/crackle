import os
import json
import threading
import atexit
from typing import Any, Dict, List, Optional

import numpy as np

SEARCH_POSES_FILE = os.path.expanduser(
    "~/crackle_ws/src/crackle/crackle_planning/crackle_planning/search_poses.json"
)

ROS_ENABLED = os.environ.get("ROS_ENABLED", "false").lower() == "true"
if ROS_ENABLED:
    from crackle_planning.sim_ros_interface import SimulatedRosInterface
else:
    from sim_ros_interface import SimulatedRosInterface


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
    def __init__(
        self,
        use_ros: bool,
        ros_node: Optional[Any] = None,
        sim: Optional["SimulatedRosInterface"] = None,
    ):
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
                if ROS_ENABLED:
                    from crackle_planning.ros_interface import RosInterface
                else:
                    from ros_interface import RosInterface
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
                self._spin_thread = threading.Thread(
                    target=self._executor.spin, daemon=True
                )
                self._spin_thread.start()

            atexit.register(self._shutdown_ros)

        else:
            # Use the provided sim instance, or create a default one.
            self.ros_interface = sim if sim is not None else SimulatedRosInterface()
            self._crackle_node = ros_node
            print("PlannerAPI running in simulation mode (ROS disabled).")

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

    def embedding_from_string(
        self, text: str, model: str = "text-embedding-3-small"
    ) -> np.ndarray:
        """Get an embedding as a NumPy vector."""
        client = _create_openai_client()
        resp = client.embeddings.create(model=model, input=text)
        return np.array(resp.data[0].embedding, dtype=np.float32)

    def recommendations_from_strings(
        self,
        strings: List[str],
        index_of_source_string: int,
        model: str = "text-embedding-3-small",
    ) -> Optional[str]:
        """Return nearest neighbors of a given string (including itself first)."""
        embeddings = [self.embedding_from_string(s, model=model) for s in strings]
        query = embeddings[index_of_source_string]
        q_norm = np.linalg.norm(query)
        distances = []
        for e in embeddings:
            denom = q_norm * np.linalg.norm(e)
            sim = float(query @ e / denom) if denom != 0 else 0.0
            distances.append(1.0 - sim)
        indices_of_nearest_neighbors = list(np.argsort(distances))
        closest_idx = indices_of_nearest_neighbors[1]
        if distances[closest_idx] <= 0.5:
            return strings[closest_idx]
        return None

    def _find_similar_object(
        self, target: str, scene_objects: List[str]
    ) -> Optional[str]:
        """Use embedding similarity to find the closest matching object in the scene."""
        if not scene_objects:
            return None
        return self.recommendations_from_strings(
            scene_objects + [target],
            len(scene_objects),
        )

    def _load_search_poses(self):
        """Load the hardcoded list of end-effector search poses from JSON."""
        from geometry_msgs.msg import Pose
        import math
        try:
            with open(SEARCH_POSES_FILE, "r") as f:
                data = json.load(f)
        except FileNotFoundError:
            print(f"Search poses file not found: {SEARCH_POSES_FILE}")
            return []
        poses = []
        for entry in data.get("search_poses", []):
            pose = Pose()
            pose.position.x = float(entry["position"]["x"])
            pose.position.y = float(entry["position"]["y"])
            pose.position.z = float(entry["position"]["z"])
            roll = math.radians(float(entry["rpy"]["r"]))
            pitch = math.radians(float(entry["rpy"]["p"]))
            yaw = math.radians(float(entry["rpy"]["y"]))
            cr, sr = math.cos(roll / 2.0), math.sin(roll / 2.0)
            cp, sp = math.cos(pitch / 2.0), math.sin(pitch / 2.0)
            cy, sy = math.cos(yaw / 2.0), math.sin(yaw / 2.0)
            pose.orientation.w = cr * cp * cy + sr * sp * sy
            pose.orientation.x = sr * cp * cy - cr * sp * sy
            pose.orientation.y = cr * sp * cy + sr * cp * sy
            pose.orientation.z = cr * cp * sy - sr * sp * cy
            poses.append(pose)
        return poses

    def pick_up(self, object_name: str):
        """Pick up the named object using the robot's gripper."""
        if self.gripper_occupied:
            print("Gripper is already holding an object.")
            return
        if self.ros_interface is not None:
            scene_objects = self.ros_interface.get_scene_object_names()
            matched_name = self._find_similar_object(object_name, scene_objects)

            if matched_name is None:
                search_poses = self._load_search_poses()
                if not search_poses:
                    print("No search poses loaded; cannot scan for object.")
                else:
                    print(
                        f"Object '{object_name}' not in scene — sweeping "
                        f"{len(search_poses)} search pose(s)..."
                    )
                for i, pose in enumerate(search_poses):
                    print(f"Moving to search pose {i + 1}/{len(search_poses)}...")
                    if not self.ros_interface.move_to_pose(pose):
                        print(f"Failed to reach search pose {i + 1}; skipping.")
                        continue
                    found_names = self.ros_interface.call_find_objects([object_name])
                    if found_names:
                        matched_name = found_names[0]
                        print(f"Found '{matched_name}' at search pose {i + 1}.")
                        break

            if matched_name is None:
                print(f"Could not find object '{object_name}' in the scene.")
                return

            print(f"Matched '{object_name}' to scene object '{matched_name}'")
            self.ros_interface.clear_and_refresh_octomap()
            self.ros_interface.call_pickup_service(matched_name)
            self.gripper_occupied = True

    def look_at_sound_direction(self, wake_word_time: float):
        """Turn the robot's head to face the direction of the detected sound."""
        if self.ros_interface is not None:
            self.ros_interface.look_at_person(wake_word_time)

    def place(self):
        """Place the currently held object down and release the gripper."""
        if self.ros_interface is not None:
            self.ros_interface.clear_and_refresh_octomap()
            self.ros_interface.open_gripper()
            self.gripper_occupied = False

    def wave(self):
        """The Robot will wave at the user."""
        print("Waving...")

    def set_emotion(self, emotion: str):
        """Set the robot's displayed emotion to the given emotion string."""
        if self.ros_interface is not None:
            self.ros_interface.set_emotion(emotion)

    def close_gripper(self):
        """Close the gripper. Useful for picking up objects or shaking hands."""
        if self.ros_interface is not None:
            self.ros_interface.close_gripper()

    def open_gripper(self):
        """Open the gripper, releasing any held object."""
        if self.ros_interface is not None:
            self.ros_interface.open_gripper()

    def dance_dance(self):
        """Make the robot perform a dance routine."""
        if self.ros_interface is not None:
            self.ros_interface.dance()

    def get_global_state_value(self, key: str):
        """Retrieve a value from the planner's global state dictionary by key."""
        return self.global_state.get(key)

    def recognize_person(self):
        """Identify and recognize the person currently visible to the robot."""
        if self.ros_interface is None:
            print("ROS interface unavailable; cannot recognize person.")
            return
        names = self.ros_interface.recognize_person()
        print(f"Recognized persons: {names}")
