import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

class CameraPublisherNode(Node):
    def __init__(self):
        super().__init__('camera_publisher_node')

        # Initialize video capture (0 for the default camera)
        self.cap = cv2.VideoCapture(0)

        # Check if the camera opened successfully
        if not self.cap.isOpened():
            self.get_logger().error('Failed to open camera')
            return

        self.bridge = CvBridge()

        # Publisher for the image topic
        self.publisher = self.create_publisher(Image, 'camera/image_raw', 10)

        # Timer to publish frames at a fixed rate
        self.timer = self.create_timer(1.0 / 30.0, self.timer_callback)  # 30 FPS

    def timer_callback(self):
        ret, frame = self.cap.read()

        if not ret:
            self.get_logger().error('Failed to capture image')
            return

        # Convert the frame to a ROS Image message
        image_msg = self.bridge.cv2_to_imgmsg(frame, encoding='bgr8')

        # Publish the image message
        self.publisher.publish(image_msg)

    def __del__(self):
        # Release the video capture object when done
        if hasattr(self, 'cap') and self.cap.isOpened():
            self.cap.release()

def main(args=None):
    rclpy.init(args=args)
    node = CameraPublisherNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
