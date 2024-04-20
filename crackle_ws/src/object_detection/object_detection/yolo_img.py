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
        self.cap = cv2.VideoCapture(0)  # Initialize webcam
        self.cap.set(3, 640)  # Set width
        self.cap.set(4, 480)  # Set height

    def run_detection(self):
        ret, img = self.cap.read()
        if ret:
            results = self.model.predict(img)
            bounding_boxes = []
            for r in results:
                for box in r.boxes:
                    b = box.xyxy[0].tolist()  # get box coordinates
                    c = box.cls
                    bounding_boxes.append((b, self.model.names[int(c)]))
            print(bounding_boxes)
            # Convert bounding box details into a string to publish
            bbox_str = str(bounding_boxes)
            self.publisher_.publish(String(data=bbox_str))
        else:
            self.get_logger().error('Failed to capture frame from camera')

    def __del__(self):
        self.cap.release()

def main(args=None):
    rclpy.init(args=args)
    node = ObjectDetectionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
