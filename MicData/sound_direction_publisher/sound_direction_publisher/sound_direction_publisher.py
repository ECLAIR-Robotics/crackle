import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Vector3
import serial

class SoundDirectionPublisherNode(Node):
    def __init__(self):
        super().__init__('SoundDirectionPublisherNode')

        self.sound_direction_publisher = self.create_publisher(Vector3, 'sound_direction', 10)

        sound_direction_vector = Vector3()
        sound_direction_vector.x = 1.0
        sound_direction_vector.y = 2.0
        sound_direction_vector.z = 3.0
        self.sound_direction_publisher.publish(sound_direction_vector)
        print("here")

        # self.ser = serial.Serial('/dev/tty.usbmodem1401', baudrate=19200, timeout=1)

        # self.timer = self.create_timer(0.1, self.read_serial)

    def read_serial(self):
        if self.ser.in_waiting:
            serial_output = self.ser.readline().decode('utf-8').strip()
            if serial_output.startswith("Vector"):
                open_parenthesis = serial_output.find("(") + 1
                end_parenthesis = serial_output.find(")", open_parenthesis)
                sound_direction = serial_output[open_parenthesis:end_parenthesis].split(",")

                sound_direction_vector = Vector3()
                sound_direction_vector.x = float(sound_direction[0])
                sound_direction_vector.y = float(sound_direction[1])
                sound_direction_vector.z = float(sound_direction[2])

                print("Vector")

                self.sound_direction_publisher.publish(sound_direction_vector)

                print("published")

def main(args=None):
    rclpy.init(args=args)
    node = SoundDirectionPublisherNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
