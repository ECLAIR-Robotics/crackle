import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool, String
import serial
from serial.tools import list_ports

class ClawDegreePublisherNode(Node):
    def __init__(self):
        super().__init__('ClawDegreePublisherNode')

        self.claw_close_subscription = self.create_subscription(Bool, '/claw/command', self.claw_close_callback, 10)
        self.claw_degs_publisher = self.create_publisher(String, "/claw/joint_state", 10)

        serial_port = None

        for p in list(list_ports.comports()):
            print(p)
            if "CP2102 USB to UART Bridge Controller" in p.description.strip():
                serial_port = p.device

        if serial_port == None:
            print("ESP32 not found")

        self.ser = serial.Serial(serial_port, baudrate=9600)

        self.timer = self.create_timer(0.1, self.read_serial)

    def claw_close_callback(self, msg):
        print("Claw command received:", msg.data)
        if msg.data:
            self.ser.write("1".encode())
        else:
            self.ser.write("0".encode())

    def read_serial(self):
        while self.ser.in_waiting > 0:
            serial_output = self.ser.readline().decode('utf-8').strip()
            try:
                serial_output = int(serial_output)
            except ValueError:
                print("Invalid data received:", serial_output)
                return
            out = String()
            if serial_output == 0:
                out.data = "claw_closed"
            elif serial_output == 1:
                out.data = "claw_opened"
            self.claw_degs_publisher.publish(out)

def main(args=None):
    rclpy.init(args=args)
    print("Claw Degree Publisher Node is starting...")
    node = ClawDegreePublisherNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
