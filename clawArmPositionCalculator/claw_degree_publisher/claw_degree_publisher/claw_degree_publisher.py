import threading

import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from std_msgs.msg import String
from std_srvs.srv import SetBool
from sensor_msgs.msg import JointState

from claw_degree_publisher.crackle_claw import CrackleClaw

# Rack joints defined in crackle_description/urdf/crackle_gripper.urdf.xacro.
# Both racks are driven symmetrically, so they share the same (positive) value.
JOINT_NAMES = ["left_rack_joint", "right_rack_joint"]

# Must match the prismatic <limit upper=".."> (rack_travel) in the URDF.
RACK_TRAVEL = 0.03  # metres, fully open (0.0) .. fully closed

# How long to wait for the firmware to report a command completed before
# giving up. grip_object() closes incrementally and monitors slip, so this
# needs headroom over the worst-case close time.
COMMAND_TIMEOUT_S = 15.0


class ClawDegreePublisherNode(Node):
    """Bridges the /claw gripper interface to the CRACKLE_CLAW firmware and
    publishes the resulting jaw position as joint state.

    Interfaces:
      * service   /claw/set_gripper (std_srvs/SetBool): data=True -> close,
        False -> open. BLOCKS until the firmware reports the command finished
        (see "DONE" ack), then returns success. This is the only command
        interface — callers wait until the gripper reaches the requested state.
      * topic     /claw/joint_state (String): "claw_closed" / "claw_opened".
      * topic     /joint_states (JointState): the two prismatic rack joints,
        so robot_state_publisher animates the URDF from real servo feedback.
    """

    def __init__(self):
        super().__init__('ClawDegreePublisherNode')

        # The service blocks waiting on _done_event; the telemetry timer must
        # keep running (in a different callback group / thread) to set it.
        self._timer_cb_group = MutuallyExclusiveCallbackGroup()
        self._service_cb_group = MutuallyExclusiveCallbackGroup()

        self.claw = CrackleClaw(logger=self.get_logger())

        # Signalled by the telemetry loop whenever a "DONE" ack is received.
        self._done_event = threading.Event()
        self._last_done = None  # '1' (closed) / '0' (opened)

        self.set_gripper_service = self.create_service(
            SetBool, '/claw/set_gripper', self.set_gripper_callback,
            callback_group=self._service_cb_group)

        self.claw_state_publisher = self.create_publisher(String, "/claw/joint_state", 10)

        # The rack joints are FIXED in the URDF for now (see crackle_gripper.urdf.xacro),
        # so publishing jaw positions on /joint_states would just spam
        # robot_state_publisher with states for fixed joints. Leave it off until
        # the rack joints are made prismatic again, then flip this param to true.
        self.declare_parameter('publish_joint_states', False)
        self._publish_joint_states = self.get_parameter('publish_joint_states').value
        self.joint_state_publisher = (
            self.create_publisher(JointState, "/joint_states", 10)
            if self._publish_joint_states else None
        )

        self.timer = self.create_timer(
            0.05, self.read_serial, callback_group=self._timer_cb_group)

    # -- telemetry ------------------------------------------------------------
    def read_serial(self):
        result = self.claw.poll()

        if result.angle is not None:
            fraction = CrackleClaw.angle_to_fraction(result.angle)

            if self.joint_state_publisher is not None:
                position = fraction * RACK_TRAVEL
                js = JointState()
                js.header.stamp = self.get_clock().now().to_msg()
                js.name = JOINT_NAMES
                js.position = [position, position]
                self.joint_state_publisher.publish(js)

            state = String()
            state.data = "claw_closed" if fraction > 0.5 else "claw_opened"
            self.claw_state_publisher.publish(state)

        if result.done is not None:
            self._last_done = result.done
            self._done_event.set()

    # -- command interfaces ---------------------------------------------------
    def _send(self, close):
        return self.claw.close() if close else self.claw.open()

    def set_gripper_callback(self, request, response):
        """Blocking open/close: returns once the firmware reports completion."""
        action = "CLOSE" if request.data else "OPEN"
        self.get_logger().info(f"Gripper service: {action}")

        self._done_event.clear()
        if not self._send(request.data) or not self.claw.connected:
            response.success = False
            response.message = "gripper not connected"
            return response

        expected = '1' if request.data else '0'
        if not self._done_event.wait(timeout=COMMAND_TIMEOUT_S):
            response.success = False
            response.message = f"timed out waiting for gripper to {action.lower()}"
            self.get_logger().warn(response.message)
            return response

        response.success = (self._last_done == expected)
        response.message = "closed" if self._last_done == '1' else "opened"
        return response


def main(args=None):
    rclpy.init(args=args)
    print("Claw Degree Publisher Node is starting...")
    node = ClawDegreePublisherNode()
    executor = MultiThreadedExecutor()
    executor.add_node(node)
    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
