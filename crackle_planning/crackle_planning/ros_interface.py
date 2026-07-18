from typing import List, Optional
import time
import rclpy
import numpy as np
from crackle_interfaces.srv import (
    FindObjects,
    PickupObject,
    LookAt,
    PlanPose,
    ExecutePlan,
    DemoTrajectory,
    SetJointAngles,
)
from moveit_msgs.srv import GetPlanningScene


from sensor_msgs.msg import Image, JointState
from rclpy.callback_groups import ReentrantCallbackGroup
from geometry_msgs.msg import Vector3Stamped
from std_msgs.msg import String
from visualization_msgs.msg import Marker
from std_srvs.srv import Trigger, Empty, SetBool
from rclpy.node import Node
from rclpy.time import Time

try:
    from crackle_planning._config import load_config
except ImportError:  # running from source without the package installed
    from _config import load_config


class AudioDirectionWindow:
    class AudioDirectionEntry:
        def __init__(self, timestamp, direction: Vector3Stamped):
            self.timestamp = timestamp
            self.direction = direction

    def __init__(self, max_size: int):
        self._max_size = max_size
        self._data: List[AudioDirectionWindow.AudioDirectionEntry] = []

    def add_direction(self, direction: Vector3Stamped, node: Node):
        if len(self._data) >= self._max_size:
            self._data.pop(0)
        # use message header time; fall back to ROS clock now if header is zero
        t_msg = Time.from_msg(direction.header.stamp)
        ts = t_msg.nanoseconds / 1e9
        if ts == 0.0:
            ts = node.get_clock().now().nanoseconds / 1e9
        self._data.append(AudioDirectionWindow.AudioDirectionEntry(ts, direction))

    def find_closest_direction(self, target_time: float):
        if not self._data:
            return None
        return min(self._data, key=lambda e: abs(e.timestamp - target_time))

    def get_directions(self):
        return self._data

    def latest(self):
        return self._data[-1] if self._data else None


class RosInterface:

    FACE_MATCH_THRESHOLD = 0.7

    def __init__(self, node: Node):
        self._node = node
        self._latest_image = None
        # Lazily-created cv_bridge for encoding the latest color frame.
        self._cv_bridge = None
        self._pickup_service_client = node.create_client(
            PickupObject, "crackle_manipulation/pickup_object"
        )
        self.latest_audio_direction = AudioDirectionWindow(
            50
        )  # 50 sample window that consists of a timestamp and a Vector

        # Snapshot of the user's audio direction captured at wake-word time (see
        # capture_user_direction). The reach-toward-user actions (fist bump, high
        # five, handover) prefer this over the continuously-updated latest sample,
        # so they aim at where the user was when they spoke rather than wherever
        # the newest (possibly stale/ambient) vector points. None until captured.
        self.captured_user_direction: Optional[Vector3Stamped] = None

        self.__multi_callback_group = ReentrantCallbackGroup()

        self.__audio_direction_subscriber = node.create_subscription(
            Vector3Stamped,
            "/sound_direction_pub/vector",
            self.update_audio_direction,
            10,
            callback_group=self.__multi_callback_group,
        )

        self.__color_image_subscriber = node.create_subscription(
            Image,
            "/camera/camera/color/image_raw",
            self.update_color_image,
            10,
            callback_group=self.__multi_callback_group,
        )

        # Latest arm joint positions, so gestures can be built as small offsets
        # from wherever the arm currently is (see perform_gesture).
        self._latest_joint_state: Optional[JointState] = None
        self.__joint_state_subscriber = node.create_subscription(
            JointState,
            "/joint_states",
            self.update_joint_state,
            10,
            callback_group=self.__multi_callback_group,
        )

        # Canned expressive motions (perform_gesture) and air-drawn shapes
        # (run_demo_trajectory) reuse the manipulation node's joint + demo services.
        self.__set_joint_angles_client = node.create_client(
            SetJointAngles, "crackle_manipulation/set_joint_angles"
        )
        self.__demo_trajectory_client = node.create_client(
            DemoTrajectory, "crackle_manipulation/demo_trajectory"
        )

        self.__look_at_client = node.create_client(
            LookAt, "crackle_manipulation/look_at"
        )

        self.__look_at_marker_publisher = node.create_publisher(
            Marker, "planner/look_at_marker", 10
        )

        # Persistent marker showing the configured fixed look position, published
        # on a timer from startup so it appears in rviz as soon as the stack is up
        # (and so late-joining rviz sessions still pick it up). Re-reads the config
        # each tick, so hand edits to fixed_look_direction move the marker live.
        self.__fixed_look_marker_publisher = node.create_publisher(
            Marker, "planner/fixed_look_marker", 10
        )
        self.__fixed_look_marker_timer = node.create_timer(
            2.0, self.publish_fixed_look_marker,
            callback_group=self.__multi_callback_group,
        )

        self.__dance_client = node.create_client(Trigger, "/crackle_manipulation/dance")

        self.__emotion_publisher = node.create_publisher(String, "/face/emotion", 10)

        self.__clear_octomap_client = node.create_client(Empty, "/clear_octomap")

        self.__scan_client = node.create_client(Trigger, "vision/scan")

        self.__get_planning_scene_client = node.create_client(
            GetPlanningScene, "/get_planning_scene"
        )

        # Blocking close/open service that waits until the gripper firmware
        # reports the command finished.
        self.__claw_set_client = node.create_client(
            SetBool, "/claw/set_gripper", callback_group=self.__multi_callback_group
        )

        self.__find_objects_client = node.create_client(
            FindObjects, "vision/find_objects"
        )

        self.__plan_pose_client = node.create_client(
            PlanPose, "crackle_manipulation/plan_pose"
        )

        self.__execute_plan_client = node.create_client(
            ExecutePlan, "crackle_manipulation/execute_plan"
        )

        # MoveIt's execution-event topic. Publishing a "stop" String makes the
        # move_group node's TrajectoryExecutionManager cancel the active
        # controller goal, aborting any trajectory currently executing. This is
        # handled by move_group (not the manipulation node's mutually-exclusive
        # service group), so it preempts an in-flight blocking execute_plan.
        self.__stop_execution_pub = node.create_publisher(
            String, "/trajectory_execution_event", 10
        )

    def stop_motion(self):
        """Instantly abort any trajectory MoveIt is currently executing.

        Publishes the MoveIt "stop" execution event. The manipulation node's
        blocking ``execute_plan`` (move_group.execute()) then returns non-success,
        so a scan move in flight unblocks promptly. Safe no-op if nothing is
        executing. Works in simulation and on the real arm.
        """
        msg = String()
        msg.data = "stop"
        self.__stop_execution_pub.publish(msg)
        self._node.get_logger().info("Published MoveIt stop execution event.")

    def update_color_image(self, msg: Image):
        """Store the latest color image from the camera."""
        self._latest_image = msg

    def get_latest_color_image_jpeg_b64(self) -> Optional[str]:
        """Return the most recent color frame as a base64-encoded JPEG string.

        Used to feed the idle-scan GPT-vision call. Returns None if no frame has
        arrived yet or the conversion fails, so callers can simply skip a frame.
        """
        msg = self._latest_image
        if msg is None:
            self._node.get_logger().info("No camera image available to capture yet.")
            return None
        try:
            import base64
            import cv2
            from cv_bridge import CvBridge

            if self._cv_bridge is None:
                self._cv_bridge = CvBridge()
            cv_img = self._cv_bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
            ok, buf = cv2.imencode(".jpg", cv_img)
            if not ok:
                self._node.get_logger().warn("Failed to JPEG-encode color image.")
                return None
            return base64.b64encode(buf.tobytes()).decode("ascii")
        except Exception as exc:
            self._node.get_logger().warn(f"Color image capture failed: {exc}")
            return None

    def spin_internal_node(self):
        """Block the calling thread and spin the internal ROS node."""
        rclpy.spin(self._node)

    def _wait_for_future(
        self, future, poll_interval: float = 0.1, timeout: float = 5.0
    ):
        start = time.time()
        while rclpy.ok() and not future.done():
            if time.time() - start > timeout:
                self._node.get_logger().warn("Service call timed out.")
                return None
            time.sleep(poll_interval)
        return future.result() if future.done() else None

    def update_audio_direction(self, direction: Vector3Stamped):
        """Buffer an incoming audio direction sample, discarding NaN vectors."""
        # check for nans
        if (
            direction.vector.x != direction.vector.x
            or direction.vector.y != direction.vector.y
            or direction.vector.z != direction.vector.z
        ):
            return
        self.latest_audio_direction.add_direction(direction, self._node)

    def update_joint_state(self, msg: JointState):
        """Cache the latest joint state (used to build gestures relative to now)."""
        self._latest_joint_state = msg

    def get_current_arm_joints(self) -> Optional[List[float]]:
        """Return [joint1..joint6] positions (radians) or None if not seen yet."""
        msg = self._latest_joint_state
        if msg is None:
            return None
        name_to_pos = dict(zip(msg.name, msg.position))
        try:
            return [float(name_to_pos[f"joint{i}"]) for i in range(1, 7)]
        except KeyError:
            return None

    def get_scene_object_names(self) -> List[str]:
        """Get the names of all collision objects currently in the MoveIt planning scene."""
        if not self.__get_planning_scene_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("GetPlanningScene service not available")
            return []
        req = GetPlanningScene.Request()
        req.components.components = 1  # WORLD_OBJECT_NAMES
        future = self.__get_planning_scene_client.call_async(req)
        result = self._wait_for_future(future, timeout=5.0)
        if result is None:
            return []
        names = [obj.id for obj in result.scene.world.collision_objects]
        self._node.get_logger().info(f"Scene objects: {names}")
        return names

    def call_scan_service(self) -> bool:
        """Trigger the vision node to scan for all objects and publish them as collision objects."""
        if not self.__scan_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("vision/scan service not available")
            return False
        req = Trigger.Request()
        future = self.__scan_client.call_async(req)
        result = self._wait_for_future(future, timeout=15.0)
        return result is not None

    def call_find_objects(self, names: List[str]) -> List[str]:
        """Call the vision find_objects service and return the names of detected objects."""
        if not self.__find_objects_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("vision/find_objects service not available")
            return []
        req = FindObjects.Request()
        req.names = names
        future = self.__find_objects_client.call_async(req)
        result = self._wait_for_future(future, timeout=15.0)
        if result is None:
            return []
        self._node.get_logger().info(f"FindObjects returned: {result.names}")
        return list(result.names)

    def move_to_pose(self, pose, abort_event=None) -> bool:
        """Plan to and execute a motion to the given end-effector pose. Blocks until the motion finishes.

        If ``abort_event`` (a threading.Event) is set before execution begins,
        the planned motion is skipped and False is returned. This covers the
        short planning window where a wake word can't be caught by ``stop_motion``
        (which only aborts a trajectory that is already executing).
        """
        if not self.__plan_pose_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("plan_pose service not available")
            return False
        plan_req = PlanPose.Request()
        plan_req.target_pose = pose
        plan_future = self.__plan_pose_client.call_async(plan_req)
        plan_result = self._wait_for_future(plan_future, timeout=15.0)
        if plan_result is None or not plan_result.success:
            self._node.get_logger().warn("plan_pose failed")
            return False

        # Don't start executing if we were interrupted during planning.
        if abort_event is not None and abort_event.is_set():
            self._node.get_logger().info("Motion aborted before execution (interrupted).")
            return False

        if not self.__execute_plan_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("execute_plan service not available")
            return False
        exec_req = ExecutePlan.Request()
        exec_req.wait = True
        exec_future = self.__execute_plan_client.call_async(exec_req)
        exec_result = self._wait_for_future(exec_future, timeout=30.0)
        if exec_result is None or not exec_result.success:
            self._node.get_logger().warn("execute_plan failed")
            return False
        return True

    def call_pickup_service(self, object_name: str) -> bool:
        """Call the manipulation pickup service for the given object name.

        Returns True only if the manipulation node reports a successful grasp.
        """
        while not self._pickup_service_client.wait_for_service(timeout_sec=1.0):
            self._node.get_logger().info("Service not available, waiting again...")
        request = PickupObject.Request()
        # The manipulation node looks this object up in the planning scene; without
        # it the request carries an empty id and pickup always fails with
        # "object '' not found in planning scene".
        request.object_name = object_name
        future = self._pickup_service_client.call_async(request)
        # Pickup covers planning + executing a full arm trajectory, so allow ample time.
        result = self._wait_for_future(future, timeout=120.0)
        if result is None:
            self._node.get_logger().warn(
                f"Pickup service for '{object_name}' timed out or returned no response."
            )
            return False
        if not result.success:
            self._node.get_logger().warn(
                f"Manipulation node failed to pick up '{object_name}'."
            )
        return bool(result.success)

    def wait_for_direction_after(self, target_time_sec: float, timeout: float = 0.5):
        """Return the first direction whose header stamp >= target_time_sec, or None on timeout."""
        deadline = time.time() + timeout
        best = None
        while time.time() < deadline:
            entry = self.latest_audio_direction.latest()
            if entry is not None:
                best = entry
                if entry.timestamp >= target_time_sec:
                    return entry
            time.sleep(0.01)  # 10ms poll while executor keeps spinning
        # Fallback: return the freshest we saw (may be slightly earlier)
        return best

    def capture_user_direction(self, wake_word_time: Optional[float] = None) -> bool:
        """Snapshot the user's audio direction (WITHOUT moving the arm).

        Called on wake word so later reach-toward-user actions (fist bump, high
        five, handover) aim at where the user was when they spoke. Waits briefly
        for a direction sample at/after ``wake_word_time`` (or takes the latest
        when None) and stores it in ``captured_user_direction``. Returns True if a
        direction was captured.
        """
        if wake_word_time is not None:
            entry = self.wait_for_direction_after(wake_word_time, timeout=0.5)
        else:
            entry = self.latest_audio_direction.latest()
        if entry is None:
            self._node.get_logger().info("capture_user_direction: no audio direction available.")
            return False
        self.captured_user_direction = entry.direction
        self._node.get_logger().info(
            f"Captured user direction: x={entry.direction.vector.x:.3f}, "
            f"y={entry.direction.vector.y:.3f}"
        )
        return True

    def _compute_look_point(self, mode: str, wake_word_time: Optional[float]):
        """Resolve the (x, y, z) point in link_base to look toward, or None.

        "fixed" -> the configured fixed_look_direction. "audio" -> the audio-
        localization vector nearest wake_word_time (or the latest one when None),
        normalized in XY and placed at a comfortable height.
        """
        flags = load_config()
        target_z = 0.55
        mag_xyplane = 0.3

        if mode == "fixed":
            fd = flags.get("fixed_look_direction", {}) or {}
            return (float(fd.get("x", 0.3)), float(fd.get("y", 0.0)), float(fd.get("z", 0.55)))

        if mode != "audio":
            self._node.get_logger().warn(f"Unknown look mode '{mode}'; using 'audio'.")
        if wake_word_time is not None:
            entry = self.wait_for_direction_after(wake_word_time, timeout=0.5)
        else:
            entry = self.latest_audio_direction.latest()
        if entry is None:
            self._node.get_logger().info("No audio direction data available to look at.")
            return None
        x, y = entry.direction.vector.x, entry.direction.vector.y
        mag_xy = (x * x + y * y) ** 0.5
        if mag_xy < 1e-6:
            self._node.get_logger().warn("XY magnitude too small; cannot look.")
            return None
        return ((x / mag_xy) * mag_xyplane, (y / mag_xy) * mag_xyplane, target_z)

    def _look_at_point(self, x: float, y: float, z: float) -> bool:
        """Call the manipulation LookAt service to aim the tool at a link_base point."""
        while not self.__look_at_client.wait_for_service(timeout_sec=1.0):
            self._node.get_logger().info("LookAt service not available, waiting again...")

        req = LookAt.Request()
        req.look_direction = Vector3Stamped()
        req.look_direction.header.stamp = self._node.get_clock().now().to_msg()
        req.look_direction.header.frame_id = "link_base"
        req.look_direction.vector.x = float(x)
        req.look_direction.vector.y = float(y)
        req.look_direction.vector.z = float(z)

        marker = Marker()
        marker.header.frame_id = "link_base"
        marker.header.stamp = self._node.get_clock().now().to_msg()
        marker.ns = "look_at_point"
        marker.id = 1
        marker.type = Marker.SPHERE
        marker.action = Marker.ADD
        marker.pose.position.x = float(x)
        marker.pose.position.y = float(y)
        marker.pose.position.z = float(z)
        marker.scale.x = marker.scale.y = marker.scale.z = 0.1
        marker.color.a = 1.0
        marker.color.g = 1.0
        self.__look_at_marker_publisher.publish(marker)

        future = self.__look_at_client.call_async(req)
        result = self._wait_for_future(future, timeout=10.0)
        return bool(result and result.success)

    def look_at_person(self, wake_word_time: Optional[float] = None, mode: Optional[str] = None):
        """Point the tool toward the user.

        The target point (link_base) is chosen by ``mode`` when supplied, else by
        the ``look_at_mode`` flag in crackle_config.json ("audio" or "fixed").
        ``mode`` lets a caller (e.g. the LLM's look_at_person action) override the
        config per call so the arm can choose where to look.
        """
        flags = load_config()
        resolved = mode if mode is not None else flags.get("look_at_mode", "audio")
        point = self._compute_look_point(resolved, wake_word_time)
        if point is None:
            return None
        return self._look_at_point(*point)

    def look_at_point(self, x: float, y: float, z: float) -> bool:
        """Aim the tool toward an explicit link_base point (used by point_at)."""
        return self._look_at_point(x, y, z)

    def publish_fixed_look_marker(self):
        """Publish a persistent marker at the configured fixed look position.

        Runs on a startup timer so the point is visible in rviz as soon as the
        stack is up. Re-reads the config each tick so hand edits move it live.
        """
        flags = load_config()
        fd = flags.get("fixed_look_direction", {}) or {}
        x = float(fd.get("x", 0.3))
        y = float(fd.get("y", 0.0))
        z = float(fd.get("z", 0.55))
        stamp = self._node.get_clock().now().to_msg()

        sphere = Marker()
        sphere.header.frame_id = "link_base"
        sphere.header.stamp = stamp
        sphere.ns = "fixed_look_point"
        sphere.id = 0
        sphere.type = Marker.SPHERE
        sphere.action = Marker.ADD
        sphere.pose.position.x = x
        sphere.pose.position.y = y
        sphere.pose.position.z = z
        sphere.pose.orientation.w = 1.0
        sphere.scale.x = sphere.scale.y = sphere.scale.z = 0.08
        sphere.color.a = 0.9
        sphere.color.r = 1.0
        sphere.color.g = 0.5
        sphere.color.b = 0.0  # orange, distinct from the green look-at marker
        self.__fixed_look_marker_publisher.publish(sphere)

        label = Marker()
        label.header.frame_id = "link_base"
        label.header.stamp = stamp
        label.ns = "fixed_look_point"
        label.id = 1
        label.type = Marker.TEXT_VIEW_FACING
        label.action = Marker.ADD
        label.pose.position.x = x
        label.pose.position.y = y
        label.pose.position.z = z + 0.09
        label.pose.orientation.w = 1.0
        label.scale.z = 0.05  # text height
        label.color.a = 1.0
        label.color.r = 1.0
        label.color.g = 0.6
        label.color.b = 0.0
        label.text = "fixed look"
        self.__fixed_look_marker_publisher.publish(label)

    def point_at_direction(self, mode: str) -> bool:
        """Aim the tool toward the 'audio' or 'fixed' direction. False if unavailable."""
        point = self._compute_look_point(mode, None)
        if point is None:
            return False
        return self._look_at_point(*point)

    def get_scene_object_poses(self) -> dict:
        """Return {object_id: (x, y, z)} for collision objects in the planning scene."""
        if not self.__get_planning_scene_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("GetPlanningScene service not available")
            return {}
        req = GetPlanningScene.Request()
        req.components.components = 1 | 2  # WORLD_OBJECT_NAMES | WORLD_OBJECT_GEOMETRY
        future = self.__get_planning_scene_client.call_async(req)
        result = self._wait_for_future(future, timeout=5.0)
        if result is None:
            return {}
        poses = {}
        for obj in result.scene.world.collision_objects:
            p = obj.pose.position
            poses[obj.id] = (p.x, p.y, p.z)
        return poses

    def set_joint_angles(self, angles: List[float]) -> bool:
        """Command the arm to the given joint angles (radians, joint1..joint6)."""
        if not self.__set_joint_angles_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("set_joint_angles service not available")
            return False
        req = SetJointAngles.Request()
        req.joint_angles = [float(a) for a in angles]
        future = self.__set_joint_angles_client.call_async(req)
        result = self._wait_for_future(future, timeout=15.0)
        return bool(result and result.success)

    def run_demo_trajectory(
        self, shape_type: str, size: float = 0.08, height: float = 0.1,
        points: int = 40, execute: bool = True,
    ) -> bool:
        """Plan (and optionally execute) an air-drawn shape via the demo service."""
        if not self.__demo_trajectory_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("demo_trajectory service not available")
            return False
        req = DemoTrajectory.Request()
        req.type = shape_type
        req.size = float(size)
        req.height = float(height)
        req.points = int(points)
        req.execute = bool(execute)
        future = self.__demo_trajectory_client.call_async(req)
        result = self._wait_for_future(future, timeout=60.0)
        if result is not None and not result.success:
            self._node.get_logger().warn(f"demo_trajectory failed: {result.message}")
        return bool(result and result.success)

    # Canned expressive motions. Each value is a list of joint-delta 6-vectors
    # (radians, joint1..joint6) applied to the pose the arm is in when the gesture
    # starts; the arm returns to that start pose afterward. Amplitudes are kept
    # modest and wrist-biased for safety — tune per your arm's calibration.
    _GESTURES = {
        "wave":       [[0.0, -0.3, 0.0, 0.0, 0.0,  0.6], [0.0, -0.3, 0.0, 0.0, 0.0, -0.6]] * 3,
        "nod":        [[0.0,  0.0, 0.0, 0.0,  0.3, 0.0], [0.0,  0.0, 0.0, 0.0, -0.3, 0.0]] * 2,
        "shake_head": [[0.0,  0.0, 0.0, 0.0, 0.0,  0.4], [0.0,  0.0, 0.0, 0.0, 0.0, -0.4]] * 2,
        "bow":        [[0.0,  0.4, 0.3, 0.0, 0.3, 0.0]],
        "shrug":      [[0.0, -0.2, 0.0, 0.0, 0.2, 0.0]],
        "flex":       [[0.0, -0.4, 0.5, 0.0, 0.4, 0.0], [0.0, -0.4, 0.5, 0.0, 0.4,  0.6],
                       [0.0, -0.4, 0.5, 0.0, 0.4, -0.6]],
        "celebrate":  [[0.0, -0.5, 0.0, 0.0, 0.0,  0.6], [0.0, -0.5, 0.0, 0.0, 0.0, -0.6]] * 3,
    }

    def gesture_names(self) -> List[str]:
        return sorted(self._GESTURES.keys())

    def perform_gesture(self, name: str) -> bool:
        """Play a named canned gesture as small joint offsets from the current pose.

        Returns to the starting pose afterward. False if the gesture is unknown
        or the arm's joint state hasn't been received yet.
        """
        key = str(name).strip().lower().replace(" ", "_").replace("-", "_")
        waypoints = self._GESTURES.get(key)
        if waypoints is None:
            self._node.get_logger().warn(
                f"Unknown gesture '{name}'. Known: {self.gesture_names()}"
            )
            return False
        start = self.get_current_arm_joints()
        if start is None:
            self._node.get_logger().warn("No joint state yet; cannot perform gesture.")
            return False

        ok = True
        for delta in waypoints:
            target = [s + d for s, d in zip(start, delta)]
            if not self.set_joint_angles(target):
                ok = False
                break
            time.sleep(0.25)
        # Always try to return to where we started, even if a waypoint failed.
        self.set_joint_angles(start)
        return ok

    @staticmethod
    def _pose_from_xyz_rpy(x, y, z, roll, pitch, yaw):
        """Build a geometry_msgs/Pose from a position and rpy (radians).

        Uses the same rpy->quaternion convention as the scan-pose loader, so
        orientations round-trip with search_poses.json / idle_scan_poses.json.
        """
        import math
        from geometry_msgs.msg import Pose

        pose = Pose()
        pose.position.x = float(x)
        pose.position.y = float(y)
        pose.position.z = float(z)
        cr, sr = math.cos(roll / 2.0), math.sin(roll / 2.0)
        cp, sp = math.cos(pitch / 2.0), math.sin(pitch / 2.0)
        cy, sy = math.cos(yaw / 2.0), math.sin(yaw / 2.0)
        pose.orientation.w = cr * cp * cy + sr * sp * sy
        pose.orientation.x = sr * cp * cy - cr * sp * sy
        pose.orientation.y = cr * sp * cy + sr * cp * sy
        pose.orientation.z = cr * cp * sy - sr * sp * cy
        return pose

    def hand_object_to_person(self, mode: Optional[str] = None) -> bool:
        """Present a held object to the user for a handover.

        Moves the end-effector to a point in the direction of the user, pauses
        briefly, then extends a little further from the base as if offering the
        object. The direction is chosen by ``mode`` (falls back to config
        ``look_at_mode``):
          * "fixed" — toward the configured ``fixed_look_direction``.
          * "audio" — toward the latest audio-localization vector.

        Does NOT open the gripper — the user asks to open it to take the object.
        Returns True only if both reach motions succeeded.
        """
        import math

        flags = load_config()
        resolved = mode if mode is not None else flags.get("look_at_mode", "audio")
        hv = flags.get("handover", {}) or {}
        radius = float(hv.get("radius", 0.35))
        extend = float(hv.get("extend", 0.12))
        height = float(hv.get("height", 0.45))
        rpy = hv.get("rpy", {}) or {}
        roll = math.radians(float(rpy.get("r", 180.0)))
        pitch = math.radians(float(rpy.get("p", 0.0)))
        yaw_offset = math.radians(float(rpy.get("y", 0.0)))

        # Unit XY direction toward the user.
        direction = self._user_direction_xy(resolved)
        if direction is None:
            self._node.get_logger().warn(
                "hand_object_to_person: no usable direction toward the user."
            )
            return False
        ux, uy = direction
        # Aim the gripper toward the user (yaw), with the configured yaw as offset.
        yaw = math.atan2(uy, ux) + yaw_offset

        present = self._pose_from_xyz_rpy(ux * radius, uy * radius, height, roll, pitch, yaw)
        self._node.get_logger().info(
            f"hand_object_to_person: presenting at ({ux*radius:.2f}, {uy*radius:.2f}, {height:.2f})"
        )
        if not self.move_to_pose(present):
            self._node.get_logger().warn("hand_object_to_person: present move failed.")
            return False

        time.sleep(0.5)

        r2 = radius + extend
        offer = self._pose_from_xyz_rpy(ux * r2, uy * r2, height, roll, pitch, yaw)
        if not self.move_to_pose(offer):
            self._node.get_logger().warn("hand_object_to_person: offer move failed.")
            return False
        return True

    def _user_direction_xy(self, mode: Optional[str] = None):
        """Return a unit (x, y) pointing toward the user, or None.

        "fixed" uses the configured fixed_look_direction; anything else uses the
        audio direction snapshotted at wake word (``captured_user_direction``),
        falling back to the latest audio-localization vector. Shared by the
        reach-toward-user actions (handover, fist bump, high five).
        """
        flags = load_config()
        resolved = mode if mode is not None else flags.get("look_at_mode", "audio")
        if resolved == "fixed":
            fd = flags.get("fixed_look_direction", {}) or {}
            dx, dy = float(fd.get("x", 0.3)), float(fd.get("y", 0.0))
        else:
            # Prefer the wake-word snapshot; fall back to the latest live sample.
            direction = self.captured_user_direction
            if direction is None:
                entry = self.latest_audio_direction.latest()
                if entry is None:
                    return None
                direction = entry.direction
            dx, dy = direction.vector.x, direction.vector.y
        mag = (dx * dx + dy * dy) ** 0.5
        if mag < 1e-6:
            return None
        return (dx / mag, dy / mag)

    def fist_bump(self, mode: Optional[str] = None) -> bool:
        """Offer a fist bump: close the gripper (a fist), reach toward the user,
        give a small forward jab, then settle back. Open-loop (no contact
        sensing). Returns True if the reach motions succeeded.
        """
        import math

        flags = load_config()
        resolved = mode if mode is not None else flags.get("look_at_mode", "audio")
        g = flags.get("greeting", {}) or {}
        radius = float(g.get("radius", 0.34))
        jab = float(g.get("jab", 0.07))
        height = float(g.get("bump_height", 0.35))
        rpy = g.get("rpy", {}) or {}
        roll = math.radians(float(rpy.get("r", 180.0)))
        pitch = math.radians(float(rpy.get("p", 0.0)))
        yaw_offset = math.radians(float(rpy.get("y", 0.0)))

        direction = self._user_direction_xy(resolved)
        if direction is None:
            self._node.get_logger().warn("fist_bump: no usable direction toward the user.")
            return False
        ux, uy = direction
        yaw = math.atan2(uy, ux) + yaw_offset

        # Make a fist first.
        self._set_gripper(True)

        ready = self._pose_from_xyz_rpy(ux * radius, uy * radius, height, roll, pitch, yaw)
        if not self.move_to_pose(ready):
            self._node.get_logger().warn("fist_bump: reach failed.")
            return False
        # Small forward jab and back.
        rj = radius + jab
        jab_pose = self._pose_from_xyz_rpy(ux * rj, uy * rj, height, roll, pitch, yaw)
        self.move_to_pose(jab_pose)
        self.move_to_pose(ready)
        return True

    def high_five(self, mode: Optional[str] = None) -> bool:
        """Offer a high five: open the gripper (an open hand), reach up toward the
        user, and hold briefly. Open-loop (no contact sensing). Returns True if
        the reach succeeded.
        """
        import math

        flags = load_config()
        resolved = mode if mode is not None else flags.get("look_at_mode", "audio")
        g = flags.get("greeting", {}) or {}
        radius = float(g.get("radius", 0.34))
        height = float(g.get("five_height", 0.5))
        rpy = g.get("rpy", {}) or {}
        roll = math.radians(float(rpy.get("r", 180.0)))
        pitch = math.radians(float(rpy.get("p", 0.0)))
        yaw_offset = math.radians(float(rpy.get("y", 0.0)))

        direction = self._user_direction_xy(resolved)
        if direction is None:
            self._node.get_logger().warn("high_five: no usable direction toward the user.")
            return False
        ux, uy = direction
        yaw = math.atan2(uy, ux) + yaw_offset

        # Open hand for the palm.
        self._set_gripper(False)

        up = self._pose_from_xyz_rpy(ux * radius, uy * radius, height, roll, pitch, yaw)
        if not self.move_to_pose(up):
            self._node.get_logger().warn("high_five: reach failed.")
            return False
        time.sleep(0.5)
        return True

    def clear_and_refresh_octomap(self, settle_time: float = 1.5):
        """Clear MoveIt's OctoMap and wait for fresh depth data to repopulate.

        Called before every pick or place so the planner sees an up-to-date
        3-D collision map of any objects YOLO didn't detect.

        OctoMap is currently DISABLED (see crackle_moveit/config/sensors_3d.yaml),
        so this is a no-op — clearing a non-existent octomap would only add a
        per-pick service-wait/settle delay. Remove this early return to restore the
        refresh once octomap sensing is re-enabled.
        """
        return

        if not self.__clear_octomap_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn(
                "clear_octomap service not available — skipping OctoMap refresh"
            )
            return
        future = self.__clear_octomap_client.call_async(Empty.Request())
        self._wait_for_future(future, timeout=3.0)
        self._node.get_logger().info(
            f"OctoMap cleared — waiting {settle_time}s for repopulation..."
        )
        time.sleep(settle_time)
        self._node.get_logger().info("OctoMap refreshed, proceeding with motion")

    def dance(self):
        """Trigger the dance service to perform a dance routine."""
        self._node.get_logger().info("Executing dance maneuver...")
        # Implement dance maneuver logic here
        while not self.__dance_client.wait_for_service(timeout_sec=1.0):
            self._node.get_logger().info(
                "Dance service not available, waiting again..."
            )
        req = Trigger.Request()
        future = self.__dance_client.call_async(req)
        return self._wait_for_future(future, timeout=20.0)

    def set_emotion(self, emotion: str):
        """Publish the given emotion string to the face display topic."""
        self._node.get_logger().info(f"Setting robot emotion to: {emotion}")
        # Implement emotion setting logic here
        msg = String()
        msg.data = emotion
        self.__emotion_publisher.publish(msg)

    def close_gripper(self) -> bool:
        """Close the claw gripper and block until the firmware reports it is
        closed. Returns True on confirmed close."""
        self._node.get_logger().info("Closing gripper...")
        return self._set_gripper(True)

    def open_gripper(self) -> bool:
        """Open the claw gripper and block until the firmware reports it is
        open. Returns True on confirmed open."""
        self._node.get_logger().info("Opening gripper...")
        return self._set_gripper(False)

    def _set_gripper(self, close: bool) -> bool:
        if not self.__claw_set_client.wait_for_service(timeout_sec=2.0):
            self._node.get_logger().warn("/claw/set_gripper service unavailable")
            return False
        req = SetBool.Request()
        req.data = close
        future = self.__claw_set_client.call_async(req)
        result = self._wait_for_future(future, timeout=20.0)
        if result is None:
            return False
        if not result.success:
            self._node.get_logger().warn(f"Gripper command failed: {result.message}")
        return result.success

    def recognize_person(self) -> List[str]:
        """Attempt to identify people in the latest camera image using face recognition."""
        try:
            import face_recognition  # type: ignore[import-not-found]
        except ImportError:
            self._node.get_logger().warn(
                "face_recognition is not installed; skipping recognize_person."
            )
            return []

        if self._latest_image is None:
            self._node.get_logger().info("No camera image available yet.")
            return []

        # NOTE: Conversion bridge is not wired in this class currently.
        # Keep this method non-fatal for now so other API calls still work.
        self._node.get_logger().warn(
            "recognize_person is not fully configured (missing image bridge); skipping."
        )
        return []

        face_locations = face_recognition.face_locations(
            self.bridge.imgmsg_to_cv2(self.color_image, desired_encoding="bgr8"),
            model="cnn",
        )
        print(f"Detected {len(face_locations)} face(s)")
        print(face_locations)
        if not face_locations:
            self.get_logger().info("No faces detected")
            return []
        face_encodings = face_recognition.face_encodings(
            self.bridge.imgmsg_to_cv2(self.color_image, desired_encoding="bgr8"),
            face_locations,
        )
        recognized_names: List[str] = []
        for face_encoding in face_encodings:
            matches = face_recognition.compare_faces(
                list(self.person_dictionary.values()), face_encoding
            )
            name = None
            face_distances = face_recognition.face_distance(
                list(self.person_dictionary.values()), face_encoding
            )
            lowest_distance = min(face_distances)
            print("Lowest distance:", lowest_distance)

        return recognized_names
