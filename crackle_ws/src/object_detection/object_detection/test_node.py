import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import ast  # Import ast module to safely evaluate strings containing Python literals

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
        # Safely evaluate the string literal received
        detection_data = ast.literal_eval(msg.data)
        self.get_logger().info('Received Detection Data: %s' % detection_data)

def main(args=None):
    rclpy.init(args=args)
    subscriber = DetectionResultsSubscriber()
    rclpy.spin(subscriber)
    subscriber.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
