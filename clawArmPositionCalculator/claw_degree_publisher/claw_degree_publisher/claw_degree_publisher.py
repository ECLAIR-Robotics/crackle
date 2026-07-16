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
RACK_TRAVEL = 0.04  # metres, fully open (0.0) .. fully closed

# Rate at which the rack joint state is republished on /joint_states, even when
# no new servo angle has arrived. robot_state_publisher and MoveIt's current-
# state monitor keep the last value per joint, so publishing continuously (at a
# steady default of 0.0 = open before the ESP32 streams) keeps TF
# (world->left_rack) resolvable and the MoveIt robot state complete. Without it,
# these prismatic joints would have no transform and planning would fail.
JOINT_STATE_PERIOD_S = 0.05

# How long to wait for the firmware to report a command completed before
# giving up. grip_object() closes incrementally and monitors slip, so this
# needs headroom over the worst-case close time.
COMMAND_TIMEOUT_S = 15.0

# Simulated-mode jaw speed: how long a full open<->close sweep takes. Only used
# when `simulated` is true; picked to look natural in rviz without being slow.
SIM_SWEEP_TIME_S = 0.4
SIM_STEP = RACK_TRAVEL * JOINT_STATE_PERIOD_S / SIM_SWEEP_TIME_S


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

    Parameters:
      * simulated (bool, default false): when true, the node does NOT open the
        serial port or depend on the ESP32 at all. It fakes the gripper — the
        service returns success once the (interpolated) racks reach the
        commanded end, so the rest of the stack behaves identically and the
        racks still animate in rviz.
      * publish_joint_states (bool, default true): publish the rack joints on
        /joint_states so TF / MoveIt see them.
    """

    def __init__(self):
        super().__init__('ClawDegreePublisherNode')

        # The service blocks waiting on _done_event; the telemetry timer must
        # keep running (in a different callback group / thread) to set it.
        self._timer_cb_group = MutuallyExclusiveCallbackGroup()
        self._service_cb_group = MutuallyExclusiveCallbackGroup()

        # In simulated mode we never touch the serial port / ESP32.
        self.declare_parameter('simulated', False)
        self._simulated = self.get_parameter('simulated').value
        if self._simulated:
            self.get_logger().info(
                "Running SIMULATED: not connecting to the CRACKLE_CLAW ESP32; "
                "faking the gripper.")
            self.claw = None
        else:
            self.claw = CrackleClaw(logger=self.get_logger())

        # Signalled by the telemetry loop whenever a command completes: a "DONE"
        # ack from the firmware (hardware) or the racks reaching the target (sim).
        self._done_event = threading.Event()
        self._last_done = None  # '1' (closed) / '0' (opened)
        # The ack token the in-flight command is waiting for ('1' close / '0'
        # open), or None when idle. Completion is only signalled for a matching
        # token, so a stale/buffered DONE from the PREVIOUS command (e.g. the
        # open issued right before a grasp) can't prematurely satisfy the wait
        # and let the arm retreat before the gripper has actually closed.
        self._expected_done = None

        self.set_gripper_service = self.create_service(
            SetBool, '/claw/set_gripper', self.set_gripper_callback,
            callback_group=self._service_cb_group)

        self.claw_state_publisher = self.create_publisher(String, "/claw/joint_state", 10)

        # The rack joints are PRISMATIC (see crackle_gripper.urdf.xacro), so their
        # position must be published on /joint_states for TF and MoveIt to have a
        # complete robot state. Publish continuously (see _tick), defaulting to
        # the fully-open position until a command/angle moves them.
        self.declare_parameter('publish_joint_states', True)
        self._publish_joint_states = self.get_parameter('publish_joint_states').value
        self.joint_state_publisher = (
            self.create_publisher(JointState, "/joint_states", 10)
            if self._publish_joint_states else None
        )

        # Latest jaw position (metres, 0.0 = open). Republished every tick so the
        # racks always have a transform even before/without live servo feedback.
        self._rack_position = 0.0
        # Commanded jaw position; the sim loop drives _rack_position toward it.
        self._target_position = 0.0

        self.timer = self.create_timer(
            JOINT_STATE_PERIOD_S, self._tick, callback_group=self._timer_cb_group)

    # -- periodic loop --------------------------------------------------------
    def _tick(self):
        """Update the jaw position (from hardware telemetry or the sim model)
        and republish the rack joint state every tick."""
        if self._simulated:
            self._sim_step()
        else:
            self._poll_hardware()

        # Republish the rack joint state every tick (not only when it changes) so
        # the prismatic joints always have a transform — this is what drives the
        # left/right racks in rviz and keeps TF/MoveIt state complete.
        if self.joint_state_publisher is not None:
            js = JointState()
            js.header.stamp = self.get_clock().now().to_msg()
            js.name = JOINT_NAMES
            js.position = [self._rack_position, self._rack_position]
            self.joint_state_publisher.publish(js)

    def _poll_hardware(self):
        result = self.claw.poll()

        # When a fresh servo angle arrives, update the cached jaw position and
        # publish the discrete open/closed state.
        if result.angle is not None:
            fraction = CrackleClaw.angle_to_fraction(result.angle)
            self._rack_position = fraction * RACK_TRAVEL
            self._publish_claw_state(fraction > 0.5)

        # Only signal completion for the ack the current command is waiting on;
        # ignore stale/buffered DONEs from a previous command.
        if result.done is not None and result.done == self._expected_done:
            self._last_done = result.done
            self._done_event.set()

    def _sim_step(self):
        # Step the racks toward the commanded target so they visibly move.
        if self._rack_position < self._target_position:
            self._rack_position = min(self._target_position,
                                      self._rack_position + SIM_STEP)
        elif self._rack_position > self._target_position:
            self._rack_position = max(self._target_position,
                                      self._rack_position - SIM_STEP)

        closed = self._rack_position > RACK_TRAVEL / 2.0
        self._publish_claw_state(closed)

        # Signal completion once the racks settle on the target (only for the
        # ack the current command is waiting on).
        settled = '1' if self._target_position > RACK_TRAVEL / 2.0 else '0'
        if abs(self._rack_position - self._target_position) < 1e-9 \
                and settled == self._expected_done and not self._done_event.is_set():
            self._last_done = settled
            self._done_event.set()

    def _publish_claw_state(self, closed):
        state = String()
        state.data = "claw_closed" if closed else "claw_opened"
        self.claw_state_publisher.publish(state)

    # -- command interfaces ---------------------------------------------------
    def _send(self, close):
        return self.claw.close() if close else self.claw.open()

    def set_gripper_callback(self, request, response):
        """Blocking open/close: returns once the gripper reaches the requested
        state (a firmware "DONE" ack, or the sim racks settling on target)."""
        action = "CLOSE" if request.data else "OPEN"
        self.get_logger().info(
            f"Gripper service ({'sim' if self._simulated else 'hw'}): {action}")

        expected = '1' if request.data else '0'
        # Arm the completion filter BEFORE issuing the command so the telemetry
        # loop only acks THIS command, and clear any prior signal.
        self._expected_done = expected
        self._done_event.clear()
        if self._simulated:
            # Just retarget the racks; the sim loop drives them there and acks.
            self._target_position = RACK_TRAVEL if request.data else 0.0
        else:
            if not self._send(request.data) or not self.claw.connected:
                response.success = False
                response.message = "gripper not connected"
                return response

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
