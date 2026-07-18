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

# Poses the arm sweeps through while IDLE to keep the planning scene populated.
# Prefer the source tree (so hand edits take effect and the colcon install space,
# which doesn't ship this data file, still finds it), falling back to the
# installed package dir.
_IDLE_SCAN_POSES_SRC = os.path.expanduser(
    "~/crackle_ws/src/crackle/crackle_planning/crackle_planning/idle_scan_poses.json"
)
IDLE_SCAN_POSES_FILE = (
    _IDLE_SCAN_POSES_SRC
    if os.path.exists(_IDLE_SCAN_POSES_SRC)
    else os.path.join(os.path.dirname(__file__), "idle_scan_poses.json")
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

    @staticmethod
    def _load_poses_from_file(path: str, key: str):
        """Load a list of end-effector poses from a JSON file.

        Each entry has a position (metres) and an rpy (DEGREES) under ``key``.
        Returns a list of geometry_msgs/Pose, or [] if the file is missing.
        """
        from geometry_msgs.msg import Pose
        import math
        try:
            with open(path, "r") as f:
                data = json.load(f)
        except FileNotFoundError:
            print(f"Poses file not found: {path}")
            return []
        poses = []
        for entry in data.get(key, []):
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

    # Named scan-pose sets (see search_poses.json). The LLM chooses an area when
    # looking for something: table items vs far-away things.
    _SCAN_SET_KEYS = {
        "table": "table_scan_poses",
        "distance": "distance_scan_poses",
    }

    @staticmethod
    def _resolve_scan_area(area) -> str:
        """Normalize a free-form area string to 'table' or 'distance'."""
        a = str(area or "table").strip().lower()
        if a in ("distance", "far", "faraway", "far_away", "away", "distant",
                 "over there", "over_there", "overthere", "room", "across"):
            return "distance"
        return "table"

    def _load_scan_set(self, area="table"):
        """Load the scan poses for the requested area ('table' or 'distance')."""
        resolved = self._resolve_scan_area(area)
        poses = self._load_poses_from_file(SEARCH_POSES_FILE, self._SCAN_SET_KEYS[resolved])
        # Backward compatibility: the table set used to live under 'search_poses'.
        if not poses and resolved == "table":
            poses = self._load_poses_from_file(SEARCH_POSES_FILE, "search_poses")
        return poses

    def _load_search_poses(self):
        """Load the table scan-pose set (default for pick-up scanning)."""
        return self._load_scan_set("table")

    def load_idle_scan_poses(self):
        """Load the poses the arm sweeps through while idle (see idle_scan_poses.json)."""
        return self._load_poses_from_file(IDLE_SCAN_POSES_FILE, "idle_scan_poses")

    def move_to_scan_pose(self, pose, abort_event=None) -> bool:
        """Move the arm to one idle-scan pose. Blocks until the motion finishes.

        If ``abort_event`` (a threading.Event) is set before execution starts,
        the move is skipped so a wake word during planning doesn't trigger a
        motion. An already-executing trajectory is aborted separately via
        ``stop_motion``.
        """
        if self.ros_interface is None:
            return False
        return self.ros_interface.move_to_pose(pose, abort_event=abort_event)

    def stop_motion(self):
        """Instantly abort any arm trajectory MoveIt is currently executing."""
        if self.ros_interface is not None:
            self.ros_interface.stop_motion()

    def capture_scene_image(self) -> Optional[str]:
        """Grab the latest color frame as a base64 JPEG (None if unavailable)."""
        if self.ros_interface is None:
            return None
        return self.ros_interface.get_latest_color_image_jpeg_b64()

    def find_and_add_objects(self, names: List[str]) -> List[str]:
        """Run find_objects for the given names; detections are published to the
        MoveIt planning scene as collision objects. Returns the detected names."""
        if self.ros_interface is None or not names:
            return []
        return self.ros_interface.call_find_objects(names)

    def look_for_objects(self, object_description: str, area: str):
        """Look around for an object by sweeping a set of scan viewpoints, adding
        it to the scene if found. Choose 'area' by where the thing is:
          * 'table' — scans viewpoints looking down at the tabletop. Use for
            things resting on the table the user might want picked up (a cup, a
            red box, etc.).
          * 'distance' — scans viewpoints looking farther out and up. Use for
            things across the room, e.g. when the user asks 'what is that over
            there'.
        Returns whether the object was found and its scene name."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        resolved = self._resolve_scan_area(area)
        matched = self._scan_for_object(object_description, area=resolved)
        if matched is None:
            return {
                "success": False,
                "message": (
                    f"I scanned the {resolved} area but couldn't find "
                    f"'{object_description}'."
                ),
            }
        return {
            "success": True,
            "matched_object": matched,
            "message": f"Found '{matched}' while scanning the {resolved} area.",
        }

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

    def _scan_for_object(self, object_name: str, area="table") -> Optional[str]:
        """Locate ``object_name`` by scanning, adding it to the planning scene.

        ``area`` selects which scan-pose set to sweep: 'table' (tabletop items) or
        'distance' (far-away things). See search_poses.json.

        Strategy:
          1. Call find_objects at the arm's CURRENT position first. If the object
             is detected (and thus added to the planning scene), return it
             without moving the arm at all.
          2. Otherwise sweep the chosen scan-pose set. Motion and detection are
             pipelined: after arriving at a pose we kick off find_objects in a
             background thread and immediately start moving to the next pose, only
             blocking on a pose's find_objects result right before scanning at the
             next pose.

        Note: find_objects captures the scene when the service call begins (arm
        stationary at the just-reached pose); the overlapping motion is the move
        toward the *next* pose, so each detection still runs from a settled view.

        Returns the matched scene-object name, or None if not found.
        """
        resolved = self._resolve_scan_area(area)

        # 1) Scan the current position before moving anywhere.
        matched = self._scan_here(object_name)
        if matched is not None:
            print(f"Found '{matched}' at the current arm position.")
            return matched

        # 2) Sweep the chosen scan-pose set with pipelined move + detection.
        search_poses = self._load_scan_set(resolved)
        if not search_poses:
            print(f"No {resolved} scan poses loaded; cannot scan for the object.")
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

    def capture_sound_direction(self, wake_word_time: float):
        """Snapshot the user's direction at wake word WITHOUT moving the arm, so
        later reach-toward-user actions (fist bump, high five, handover) aim at
        where the user was when they spoke."""
        if self.ros_interface is not None:
            self.ros_interface.capture_user_direction(wake_word_time)

    def place(self):
        """Place the currently held object down and release the gripper."""
        if self.ros_interface is not None:
            self.ros_interface.clear_and_refresh_octomap()
            self.ros_interface.open_gripper()
            self.gripper_occupied = False

    def wave(self):
        """Wave hello at the user with the arm."""
        if self.ros_interface is None:
            print("Waving...")
            return {"success": True, "message": "Waved hello!"}
        ok = self.ros_interface.perform_gesture("wave")
        return {
            "success": bool(ok),
            "message": "Waved hello!" if ok else "I tried to wave but couldn't move.",
        }

    # Friendly shape name -> demo_trajectory service type.
    _SHAPE_ALIASES = {
        "circle": "circle_xy", "circle_xy": "circle_xy", "round": "circle_xy",
        "square": "square_xy", "box": "square_xy", "square_xy": "square_xy",
        "figure8": "figure8_xy", "figure_8": "figure8_xy", "figure_eight": "figure8_xy",
        "eight": "figure8_xy", "infinity": "figure8_xy", "figure8_xy": "figure8_xy",
        "helix": "helix_z", "spiral": "helix_z", "coil": "helix_z", "helix_z": "helix_z",
        "heart": "heart_xy", "love": "heart_xy", "heart_xy": "heart_xy",
        "star": "star_xy", "star_xy": "star_xy",
    }

    def gesture(self, name: str):
        """Play a short expressive gesture with the arm. Valid names: 'wave',
        'nod' (yes), 'shake_head' (no), 'bow', 'shrug', 'flex', 'celebrate'. Use
        this for playful, social motions (greeting, agreeing, showing off)."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        ok = self.ros_interface.perform_gesture(name)
        if not ok:
            known = ", ".join(self.ros_interface.gesture_names())
            return {
                "success": False,
                "message": f"I couldn't do the '{name}' gesture. I know: {known}.",
            }
        return {"success": True, "message": f"Did the {name} gesture."}

    def draw_shape_in_air(self, shape: str):
        """Draw a shape in the air with the gripper. Valid shapes: 'circle',
        'square', 'figure8', 'spiral', 'heart', 'star'. A fun way to show off the
        arm's motion."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        key = str(shape).strip().lower().replace(" ", "_").replace("-", "_")
        shape_type = self._SHAPE_ALIASES.get(key)
        if shape_type is None:
            return {
                "success": False,
                "message": (
                    f"I can draw a circle, square, figure-8, or spiral — not '{shape}'."
                ),
            }
        ok = self.ros_interface.run_demo_trajectory(shape_type)
        return {
            "success": bool(ok),
            "message": f"Drew a {shape} in the air." if ok else f"I couldn't draw the {shape}.",
        }

    def point_at(self, target: str):
        """Point the arm/gripper toward something. Pass target='audio' (or 'user')
        to point toward where the user's voice last came from, target='fixed' to
        point toward the configured spot, or the NAME of an object in the scene
        (e.g. 'red cup') to point at that object."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        t = str(target).strip().lower()
        audio_words = {"audio", "user", "sound", "speaker", "me", "you", "voice",
                       "talker", "person"}
        fixed_words = {"fixed", "forward", "front", "ahead", "default"}
        if t in audio_words:
            ok = self.ros_interface.point_at_direction("audio")
            return {
                "success": bool(ok),
                "message": "Pointing toward the user." if ok
                else "I don't have a sound direction to point at yet.",
            }
        if t in fixed_words:
            ok = self.ros_interface.point_at_direction("fixed")
            return {
                "success": bool(ok),
                "message": "Pointing toward the usual spot." if ok else "Couldn't point there.",
            }
        # Otherwise treat the target as a named scene object.
        poses = self.ros_interface.get_scene_object_poses()
        if poses:
            matched, _ = self._match_scene_object(target, list(poses.keys()))
            if matched is not None:
                x, y, z = poses[matched]
                ok = self.ros_interface.look_at_point(x, y, z)
                return {
                    "success": bool(ok),
                    "message": f"Pointing at {matched}." if ok
                    else f"I see {matched} but couldn't point at it.",
                }
        ok = self.ros_interface.point_at_direction("fixed")
        return {
            "success": bool(ok),
            "message": (
                f"I couldn't find '{target}' to point at, so I looked toward the usual spot."
            ),
        }

    def set_emotion(self, emotion: str):
        """Set the robot's displayed emotion to the given emotion string."""
        if self.ros_interface is not None:
            self.ros_interface.set_emotion(emotion)

    def close_gripper(self):
        """Close the gripper. Useful for picking up objects or shaking hands.

        Note: closing does NOT mark us as holding a tracked object — use pick_up
        to grasp-and-track. gripper_occupied only reflects objects picked up.
        """
        if self.ros_interface is not None:
            self.ros_interface.close_gripper()

    def open_gripper(self):
        """Open the gripper, releasing any held object."""
        if self.ros_interface is not None:
            self.ros_interface.open_gripper()
        # Opening always releases whatever we were holding.
        self.gripper_occupied = False

    def set_gripper_state(self, state: str):
        """Open or close the gripper. Pass state='open' to release (any held
        object is dropped) or state='close' to close the jaws. Blocks until the
        gripper firmware confirms the requested state."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        close = str(state).strip().lower().startswith("c")
        ok = (
            self.ros_interface.close_gripper()
            if close
            else self.ros_interface.open_gripper()
        )
        if not close:
            # Opening always releases whatever we were holding.
            self.gripper_occupied = False
        return {
            "success": bool(ok),
            "message": (
                "Closed the gripper." if close
                else "Opened the gripper; not holding anything now."
            ),
        }

    def look_at_person(self, target: str):
        """Turn the arm to look toward the user. Pass target='audio' to look
        toward where the user's voice last came from (the remembered audio
        direction), or target='fixed' to look toward the configured fixed point
        where a user usually stands. Call this whenever you want to make eye
        contact — e.g. greeting the user or before handing something over."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        mode = "fixed" if str(target).strip().lower().startswith("f") else "audio"
        self.ros_interface.look_at_person(wake_word_time=None, mode=mode)
        return {"success": True, "message": f"Looking toward the {mode} direction."}

    def hand_object_to_person(self, target: str):
        """Hand the currently held object to the user. Pass target='audio' to
        reach toward where the user's voice last came from, or target='fixed' to
        reach toward the configured fixed point. The arm presents the object and
        extends it toward the user but does NOT release it — after calling this,
        tell the user to say 'open gripper' and then call set_gripper_state('open')
        (or open_gripper) to release it."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        if not self.gripper_occupied:
            return {
                "success": False,
                "message": "I'm not holding anything to hand over.",
            }
        mode = "fixed" if str(target).strip().lower().startswith("f") else "audio"
        ok = self.ros_interface.hand_object_to_person(mode=mode)
        if not ok:
            return {
                "success": False,
                "message": "I couldn't reach a good handover position.",
            }
        # The object is presented but still held until the user asks to open the
        # gripper — gripper_occupied stays True until then.
        return {
            "success": True,
            "message": (
                "Offering the object to the user — tell them to say 'open gripper' "
                "and then release it."
            ),
        }

    def hold_object_from_user(self):
        """Hold an object the user wants to hand you. Call this when the user
        says something like 'hold this for me', 'can you hold this', or 'take
        this'. The arm moves to the configured hold pose (crackle_config.json
        'hold_pose') and OPENS the gripper so they can place the object into it.
        It does NOT close the gripper — after calling this, tell the user to say
        'close gripper', which you handle by calling set_gripper_state('close').
        Refuses if you're already holding something (place it first)."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        if self.gripper_occupied:
            return {
                "success": False,
                "message": "I'm already holding something — let me place it first.",
            }
        ok = self.ros_interface.go_to_hold_pose()
        if not ok:
            return {
                "success": False,
                "message": "I couldn't reach the hold position.",
            }
        return {
            "success": True,
            "message": (
                "Ready — go ahead and set it in my gripper, then say 'close "
                "gripper' and I'll hold onto it."
            ),
        }

    def fist_bump(self, target: str):
        """Give the user a fist bump. Makes a fist, reaches toward target='audio'
        (where the user's voice last came from) or target='fixed' (the configured
        spot), gives a small jab, and settles back. Fun for greetings."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        mode = "fixed" if str(target).strip().lower().startswith("f") else "audio"
        ok = self.ros_interface.fist_bump(mode=mode)
        return {
            "success": bool(ok),
            "message": "Bump! 👊" if ok else "I couldn't reach for the fist bump.",
        }

    def high_five(self, target: str):
        """Give the user a high five. Opens the hand and reaches up toward
        target='audio' or target='fixed'. Can't do it while holding an object
        (opening would drop it) — place the object first."""
        if self.ros_interface is None:
            return {"success": False, "message": "ROS interface unavailable."}
        if self.gripper_occupied:
            return {
                "success": False,
                "message": "I'm holding something — let me place it first, then I can high five.",
            }
        mode = "fixed" if str(target).strip().lower().startswith("f") else "audio"
        ok = self.ros_interface.high_five(mode=mode)
        return {
            "success": bool(ok),
            "message": "Up high! ✋" if ok else "I couldn't reach up for the high five.",
        }

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
