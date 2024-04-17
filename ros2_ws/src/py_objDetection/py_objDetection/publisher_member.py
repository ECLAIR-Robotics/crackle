import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import cv2
from ultralytics import YOLO  # Assuming you are using YOLO from Ultralytics

class ObjectDetectionNode(Node):
    def __init__(self):
        super().__init__('object_detection_node')
        self.publisher_ = self.create_publisher(String, 'detection_results', 10)
        self.model = YOLO('yolov8n.pt')  # Load the model
        self.timer = self.create_timer(0.1, self.run_detection)  # Adjust timer as needed

    def run_detection(self):
        # Dummy image source, replace with actual video feed or image processing
        img = cv2.imread('/path/to/image.jpg')
        results = self.model(img)

        # Format results as string or use a custom message type
        detection_message = String()
        detection_message.data = str(results.pred[0])
        self.publisher_.publish(detection_message)
        self.get_logger().info('Publishing detection results')

def main(args=None):
    rclpy.init(args=args)
    node = ObjectDetectionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
