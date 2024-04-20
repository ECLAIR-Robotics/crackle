import rclpy
from rclpy.node import Node
from vision_msgs.msg import Detection2DArray

class DetectionResultsSubscriber(Node):
    def __init__(self):
        super().__init__('detection_results_subscriber')
        self.subscription = self.create_subscription(
            Detection2DArray,
            'detection_results',
            self.listener_callback,
            10
        )
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        for detection in msg.detections:
            bbox = detection.bbox
            self.get_logger().info(
                f'Received Detection - Center: ({bbox.center.x:.2f}, {bbox.center.y:.2f}), '
                f'Size: ({bbox.size_x:.2f}, {bbox.size_y:.2f})'
            )

def main(args=None):
    rclpy.init(args=args)
    subscriber = DetectionResultsSubscriber()
    rclpy.spin(subscriber)
    subscriber.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
