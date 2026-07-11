import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool, String
from sensor_msgs.msg import JointState

from claw_degree_publisher.crackle_claw import CrackleClaw

# Rack joints defined in crackle_description/urdf/crackle_gripper.urdf.xacro.
# Both racks are driven symmetrically, so they share the same (positive) value.
JOINT_NAMES = ["left_rack_joint", "right_rack_joint"]

# Must match the prismatic <limit upper=".."> (rack_travel) in the URDF.
RACK_TRAVEL = 0.03  # metres, fully open (0.0) .. fully closed


class ClawDegreePublisherNode(Node):
    """Bridges the /claw/command interface to the CRACKLE_CLAW firmware and
    publishes the resulting jaw position as joint state.

    Interfaces (unchanged from the previous node):
      * subscribes /claw/command (Bool): True = close, False = open
      * publishes  /claw/joint_state (String): "claw_closed" / "claw_opened"
    Added:
      * publishes  /joint_states (JointState): the two prismatic rack joints,
        so robot_state_publisher animates the URDF from real servo feedback.
    """

    def __init__(self):
        super().__init__('ClawDegreePublisherNode')

        self.claw_close_subscription = self.create_subscription(
            Bool, '/claw/command', self.claw_close_callback, 10)
        self.claw_state_publisher = self.create_publisher(String, "/claw/joint_state", 10)
        self.joint_state_publisher = self.create_publisher(JointState, "/joint_states", 10)

        self.claw = CrackleClaw(logger=self.get_logger())

        self.timer = self.create_timer(0.1, self.read_serial)

    def claw_close_callback(self, msg):
        # /claw/command: True = close, False = open.
        if msg.data:
            self.get_logger().info("Claw command: CLOSE")
            self.claw.close()
        else:
            self.get_logger().info("Claw command: OPEN")
            self.claw.open()
        if not self.claw.connected:
            self.get_logger().warn("Claw command received but ESP32 not connected")

    def read_serial(self):
        angle = self.claw.read_angle()
        if angle is None:
            return

        fraction = CrackleClaw.angle_to_fraction(angle)
        position = fraction * RACK_TRAVEL

        # Joint state for the URDF (both racks move together).
        js = JointState()
        js.header.stamp = self.get_clock().now().to_msg()
        js.name = JOINT_NAMES
        js.position = [position, position]
        self.joint_state_publisher.publish(js)

        # Backwards-compatible discrete state string.
        state = String()
        state.data = "claw_closed" if fraction > 0.5 else "claw_opened"
        self.claw_state_publisher.publish(state)


def main(args=None):
    rclpy.init(args=args)
    print("Claw Degree Publisher Node is starting...")
    node = ClawDegreePublisherNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == '__main__':
    main()
