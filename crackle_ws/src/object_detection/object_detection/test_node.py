import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import json  # Import JSON module to parse JSON strings

class DetectionResultsSubscriber(Node):
    def __init__(self):
        super().__init__('detection_results_subscriber')
        self.subscription = self.create_subscription(
            String,
            'detection_results',
            self.listener_callback,
            10
        )
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        # Parse the JSON data received
        detection_data = json.loads(msg.data)
        self.get_logger().info(f'Received Detection Data: {detection_data}')

def main(args=None):
    rclpy.init(args=args)
    subscriber = DetectionResultsSubscriber()
    rclpy.spin(subscriber)
    subscriber.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
