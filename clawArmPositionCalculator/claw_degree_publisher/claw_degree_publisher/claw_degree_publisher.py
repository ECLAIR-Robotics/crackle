import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
import serial
from serial.tools import list_ports

class ClawDegreePublisherNode(Node):
    def __init__(self):
        super().__init__('ClawDegreePublisherNode')

        self.claw_degs_publisher = self.create_publisher(JointState, "/claw_deg_pub/joint_state", 10)

        serial_port = None

        for p in list(list_ports.comports()):
            print(p)
            if "CP2102 USB to UART Bridge Controller" in p.description.strip():
                serial_port = p.device

        if serial_port == None:
            print("ESP32 not found")

        self.ser = serial.Serial(serial_port, baudrate=9600)

        self.timer = self.create_timer(0.1, self.read_serial)

    def read_serial(self):
        while self.ser.in_waiting > 0:
            print("Raw data:", self.ser.readline())
            serial_output = self.ser.readline().decode('utf-8').strip()
            if serial_output.startswith("ClawDegs"):
                open_parenthesis = serial_output.find("(") + 1
                end_parenthesis = serial_output.find(")", open_parenthesis)
                claw_degs = serial_output[open_parenthesis:end_parenthesis].split(",")

                claw_degs_state = JointState()
                claw_degs_state.name.append("0")
                claw_degs_state.position.append(float(claw_degs[0]))
                claw_degs_state.name.append("1")
                claw_degs_state.position.append(float(claw_degs[1]))
                claw_degs_state.name.append("2")
                claw_degs_state.position.append(float(claw_degs[2]))

                self.claw_degs_publisher.publish(claw_degs_state)

                print("publishning")

def main(args=None):
    rclpy.init(args=args)
    node = ClawDegreePublisherNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
