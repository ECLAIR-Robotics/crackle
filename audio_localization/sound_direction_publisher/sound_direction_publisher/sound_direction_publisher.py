import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Vector3, Vector3Stamped, Point
from visualization_msgs.msg import Marker
import serial
from serial.tools import list_ports
import math


class SoundDirectionPublisherNode(Node):
    def __init__(self):
        super().__init__("SoundDirectionPublisherNode")

        self.sound_direction_publisher = self.create_publisher(
            Vector3Stamped, "/sound_direction_pub/vector", 10
        )
        self.sound_direction_marker_publisher = self.create_publisher(
            Marker, "/sound_direction_pub/marker", 10
        )

        serial_port = None

        for p in list(list_ports.comports()):
            if p.description.strip() == "Arduino Mega":
                self.get_logger().info(f"Found device: {p.device} - {p.description}")
                serial_port = p.device

        if serial_port == None:
            self.get_logger().error("Arduino Mega not found")

        self.ser = serial.Serial(serial_port, baudrate=19200, timeout=1)

    def read_serial(self):
        while self.ser.in_waiting > 0:
            serial_output = self.ser.readline().decode("utf-8", errors="ignore").strip()
            if serial_output.startswith("Vector"):
                open_parenthesis = serial_output.find("(") + 1
                end_parenthesis = serial_output.find(")", open_parenthesis)
                sound_direction = serial_output[open_parenthesis:end_parenthesis].split(
                    ","
                )

                sound_direction_vector = Vector3()
                sound_direction_vector.x = float(sound_direction[1])
                sound_direction_vector.y = -1.0 * float(sound_direction[0])
                sound_direction_vector.z = float(sound_direction[2])

                # Vector 3 stamped because header is required
                sound_direction_stamped = Vector3Stamped()
                sound_direction_stamped.vector = sound_direction_vector
                sound_direction_stamped.header.stamp = self.get_clock().now().to_msg()
                sound_direction_stamped.header.frame_id = "link_base"

                # Publish sound direction vector
                marker = Marker()
                marker.header.frame_id = "link_base"
                marker.header.stamp = self.get_clock().now().to_msg()
                marker.ns = "sound_direction"
                marker.id = 0
                marker.type = Marker.ARROW
                marker.action = Marker.ADD

                sound_direction_cross_z_axis = [
                    0 * float(sound_direction[2]) - 1 * float(sound_direction[1]),
                    1 * float(sound_direction[0]) - 0 * float(sound_direction[2]),
                    0 * float(sound_direction[1]) - 0 * float(sound_direction[0]),
                ]
                start_point = Point()
                start_point.x = float(0.0)
                start_point.y = float(0.0)
                start_point.z = float(0.0)

                # Rotated 90 degrees around Z axis to match robot coordinate frame
                end_point = Point()
                end_point.x = float(sound_direction[1])
                end_point.y = -1.0 * float(sound_direction[0])
                end_point.z = float(sound_direction[2])

                marker.points.append(start_point)
                marker.points.append(end_point)
                marker.scale.x = 0.1
                marker.scale.y = 0.01
                marker.scale.z = 0.01
                marker.color.a = 1.0
                marker.color.r = 1.0
                marker.color.g = 0.0
                marker.color.b = 0.0

                self.sound_direction_publisher.publish(sound_direction_stamped)
                self.sound_direction_marker_publisher.publish(marker)

                self.get_logger().info("published")


def main(args=None):
    rclpy.init(args=args)
    node = SoundDirectionPublisherNode()
    while rclpy.ok():
        node.read_serial()


if __name__ == "__main__":
    main()
