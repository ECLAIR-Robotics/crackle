import os
import json
import threading
import atexit
import concurrent.futures
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
    # Max cosine distance (1 - cosine similarity) at which a requested object name
    # is considered to match a scene object. Empirically, "red cup" vs "cup" ~= 0.54
    # and "red cup" vs "mug" ~= 0.52, so a threshold of 0.5 wrongly rejects generic
    # vision labels. 0.6 tolerates category-only labels while still excluding
    # unrelated objects ("red cup" vs "object_0" ~= 0.79).
    SIMILARITY_DISTANCE_THRESHOLD = 0.6

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

    @staticmethod
    def _normalize_name(name: str) -> str:
        """Normalize an object name for comparison: underscores -> spaces, lowercased.

        Makes matching agnostic to the underscore convention used for MoveIt ids.
        """
        return name.replace("_", " ").strip().lower()

    def _match_scene_object(
        self,
        target: str,
        scene_objects: List[str],
        threshold: Optional[float] = None,
        model: str = "text-embedding-3-small",
    ) -> tuple:
        """Find the scene object whose embedding is closest to ``target``.

        Returns ``(matched_name, distance)`` when the nearest object is within
        ``threshold`` cosine distance, otherwise ``(None, best_distance)`` so the
        caller can log how close the best candidate was. ``(None, None)`` when the
        scene is empty.
        """
        if threshold is None:
            threshold = self.SIMILARITY_DISTANCE_THRESHOLD
        if not scene_objects:
            return None, None

        # Compare on the semantic content only: treat underscores and spaces as
        # equivalent so "red_cup" and "red cup" embed identically.
        target_emb = self.embedding_from_string(self._normalize_name(target), model=model)
        target_norm = np.linalg.norm(target_emb)

        best_name: Optional[str] = None
        best_dist = float("inf")
        distances = []
        for name in scene_objects:
            emb = self.embedding_from_string(self._normalize_name(name), model=model)
            denom = target_norm * np.linalg.norm(emb)
            sim = float(target_emb @ emb / denom) if denom != 0 else 0.0
            dist = 1.0 - sim
            distances.append((name, dist))
            if dist < best_dist:
                best_name, best_dist = name, dist

        ranked = ", ".join(f"{n}={d:.3f}" for n, d in sorted(distances, key=lambda x: x[1]))
        print(f"[pick_up] embedding distances for '{target}': {ranked}")

        if best_dist <= threshold:
            return best_name, best_dist
        return None, best_dist

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

    def _scan_here(self, object_name: str) -> Optional[str]:
        """Call the find_objects service at the arm's current position.

        find_objects both detects the object and publishes it to the planning
        scene. Returns the matched (embedding-ranked) scene-object name, or None.
        """
        found_names = self.ros_interface.call_find_objects([object_name])
        if not found_names:
            return None
        matched, _ = self._match_scene_object(object_name, found_names)
        return matched or found_names[0]

    def _scan_for_object(self, object_name: str) -> Optional[str]:
        """Locate ``object_name`` by scanning, adding it to the planning scene.

        Strategy:
          1. Call find_objects at the arm's CURRENT position first. If the object
             is detected (and thus added to the planning scene), return it
             without moving the arm at all.
          2. Otherwise sweep the configured search poses. Motion and detection
             are pipelined: after arriving at a pose we kick off find_objects in
             a background thread and immediately start moving to the next pose,
             only blocking on a pose's find_objects result right before scanning
             at the next pose.

        Note: find_objects captures the scene when the service call begins (arm
        stationary at the just-reached pose); the overlapping motion is the move
        toward the *next* pose, so each detection still runs from a settled view.

        Returns the matched scene-object name, or None if not found.
        """
        # 1) Scan the current position before moving anywhere.
        matched = self._scan_here(object_name)
        if matched is not None:
            print(f"Found '{matched}' at the current arm position.")
            return matched

        # 2) Sweep the search poses with pipelined move + detection.
        search_poses = self._load_search_poses()
        if not search_poses:
            print("No search poses loaded; cannot scan the table for the object.")
            return None

        print(
            f"'{object_name}' not visible from the current position — sweeping "
            f"{len(search_poses)} search pose(s)..."
        )

        pending_scan: Optional[concurrent.futures.Future] = None
        executor = concurrent.futures.ThreadPoolExecutor(max_workers=1)
        try:
            for i, pose in enumerate(search_poses):
                print(f"Moving to search pose {i + 1}/{len(search_poses)}...")
                # Move to this pose while the previous pose's scan resolves.
                moved = self.ros_interface.move_to_pose(pose)

                # Block on the previous pose's detection before scanning again.
                if pending_scan is not None:
                    matched = pending_scan.result()
                    pending_scan = None
                    if matched is not None:
                        print(f"Found '{matched}' while sweeping search poses.")
                        return matched

                if not moved:
                    print(f"Failed to reach search pose {i + 1}; skipping its scan.")
                    continue

                # Kick off detection here; the next iteration's move overlaps it.
                pending_scan = executor.submit(self._scan_here, object_name)

            # Block on the final pose's detection.
            if pending_scan is not None:
                matched = pending_scan.result()
                if matched is not None:
                    print(f"Found '{matched}' at the final search pose.")
                    return matched
        finally:
            executor.shutdown(wait=True)

        return None

    def pick_up(self, object_name: str):
        """Pick up the named object using the robot's gripper.

        First checks whether the object is already in the planning scene using
        embedding similarity (so generic vision labels like "cup" still match a
        request for "red cup"). If it isn't already there, scans for it — first
        at the arm's current position, then sweeping the search poses (see
        ``_scan_for_object``).

        Returns a structured result so the caller/LLM knows the real outcome.
        """
        if self.gripper_occupied:
            return {
                "success": False,
                "message": "Gripper is already holding an object.",
            }
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}

        # 1) Check objects already in the scene via embedding similarity.
        scene_objects = self.ros_interface.get_scene_object_names()
        matched_name, best_dist = self._match_scene_object(object_name, scene_objects)

        if matched_name is not None:
            print(
                f"Matched '{object_name}' to scene object '{matched_name}' "
                f"(distance {best_dist:.3f})"
            )
        else:
            # 2) Not confidently in the scene — scan for it.
            if scene_objects:
                closest = f" (closest was {best_dist:.3f})" if best_dist is not None else ""
                print(
                    f"'{object_name}' not confidently in scene{closest}; "
                    f"scene objects: {scene_objects}"
                )
            matched_name = self._scan_for_object(object_name)

        if matched_name is None:
            msg = f"Could not find object '{object_name}' in the scene."
            print(msg)
            return {"success": False, "message": msg}

        self.ros_interface.clear_and_refresh_octomap()
        picked = self.ros_interface.call_pickup_service(matched_name)
        if not picked:
            # The object was located but the grasp/motion failed — report honestly
            # so the planner tells the user instead of claiming success.
            return {
                "success": False,
                "matched_object": matched_name,
                "message": (
                    f"Found '{matched_name}' in the scene but the pick-up motion "
                    "failed — I couldn't grasp it."
                ),
            }
        self.gripper_occupied = True
        return {
            "success": True,
            "matched_object": matched_name,
            "message": f"Picked up '{matched_name}' (requested '{object_name}').",
        }

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
