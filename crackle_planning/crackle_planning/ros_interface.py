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
)
from moveit_msgs.srv import GetPlanningScene


from sensor_msgs.msg import Image
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

        self.__look_at_client = node.create_client(
            LookAt, "crackle_manipulation/look_at"
        )

        self.__look_at_marker_publisher = node.create_publisher(
            Marker, "planner/look_at_marker", 10
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

    def look_at_person(self, wake_word_time: Optional[float] = None):
        """Point the robot's head toward the user after the wake word.

        The target point (in link_base) is chosen per the ``look_at_mode`` flag
        in crackle_config.json:
          * "fixed" — always look toward the configured ``fixed_look_direction``.
          * "audio" — look toward the audio-localization vector nearest the wake
            word time (the original behavior); does nothing if none is available.
        """
        flags = load_config()
        mode = flags.get("look_at_mode", "audio")

        # Desired magnitude in XY plane and target height (audio mode).
        target_z = 0.55
        mag_xyplane = 0.3

        if mode == "fixed":
            fd = flags.get("fixed_look_direction", {}) or {}
            target_point_x = float(fd.get("x", 0.3))
            target_point_y = float(fd.get("y", 0.0))
            target_point_z = float(fd.get("z", 0.55))
            self._node.get_logger().info(
                f"look_at_mode=fixed — looking at configured point: "
                f"x={target_point_x}, y={target_point_y}, z={target_point_z}"
            )
        else:
            if mode != "audio":
                self._node.get_logger().warn(
                    f"Unknown look_at_mode '{mode}'; defaulting to 'audio'."
                )
            if wake_word_time is not None:
                closest_direction_entry = self.wait_for_direction_after(
                    wake_word_time, timeout=0.5
                )
            else:
                closest_direction_entry = self.latest_audio_direction.latest()
            if closest_direction_entry is None:
                self._node.get_logger().info(
                    "No audio direction data available to look at."
                )
                return

            direction = closest_direction_entry.direction

            # Extract the input direction vector
            x, y, z = direction.vector.x, direction.vector.y, direction.vector.z

            # Compute magnitude in the XY plane
            mag_xy = (x**2 + y**2) ** 0.5
            if mag_xy < 1e-6:
                self._node.get_logger().warn("XY magnitude is too small; cannot normalize.")
                return

            # Normalize XY plane and rescale to desired magnitude
            x = (x / mag_xy) * mag_xyplane
            y = (y / mag_xy) * mag_xyplane
            z = target_z  # force target height

            target_point_x = x
            target_point_y = y
            target_point_z = z

            self._node.get_logger().info(
                f"Looking at point: x={target_point_x}, y={target_point_y}, z={target_point_z}"
            )

        while not self.__look_at_client.wait_for_service(timeout_sec=1.0):
            self._node.get_logger().info(
                "LookAt service not available, waiting again..."
            )

        req = LookAt.Request()
        req.look_direction = Vector3Stamped()
        req.look_direction.header.stamp = self._node.get_clock().now().to_msg()
        req.look_direction.header.frame_id = "link_base"
        req.look_direction.vector.x = target_point_x
        req.look_direction.vector.y = target_point_y
        req.look_direction.vector.z = target_point_z

        # Visualization marker
        marker = Marker()
        marker.header.frame_id = "link_base"
        marker.header.stamp = self._node.get_clock().now().to_msg()
        marker.ns = "look_at_point"
        marker.id = 1
        marker.type = Marker.SPHERE
        marker.action = Marker.ADD
        marker.pose.position.x = target_point_x
        marker.pose.position.y = target_point_y
        marker.pose.position.z = target_point_z
        marker.scale.x = 0.1
        marker.scale.y = 0.1
        marker.scale.z = 0.1
        marker.color.a = 1.0
        marker.color.r = 0.0
        marker.color.g = 1.0
        marker.color.b = 0.0
        self.__look_at_marker_publisher.publish(marker)

        future = self.__look_at_client.call_async(req)
        return self._wait_for_future(future, timeout=10.0)

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
