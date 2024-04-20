import rclpy
from rclpy.node import Node
from vision_msgs.msg import Detection2D, Detection2DArray, BoundingBox2D
from std_msgs.msg import Header
from sensor_msgs.msg import Image
from ultralytics import YOLO
import cv2
from ultralytics.utils.plotting import Annotator


class ObjectDetectionNode(Node):
    def __init__(self):
        super().__init__('object_detection_node')
        self.publisher_ = self.create_publisher(Detection2DArray, 'detection_results', 10)
        self.model = YOLO('yolov8n.pt')  # Load the model
        self.timer = self.create_timer(0.1, self.run_detection)  # Timer setup
        self.cap = cv2.VideoCapture(0)
        self.cap.set(3, 640)
        self.cap.set(4, 480)

    def run_detection(self):
        ret, img = self.cap.read()
        if ret:
            results = self.model.predict(img)
            detection_array = Detection2DArray()
            detection_array.header = Header(frame_id='camera_frame', stamp=self.get_clock().now().to_msg())

            for r in results:
                for box in r.boxes:
                    detection = Detection2D()
                    detection.header = Header(frame_id='camera_frame', stamp=self.get_clock().now().to_msg())
                    bbox = BoundingBox2D()
                    bbox.center.x = (box.xyxy[0] + box.xyxy[2]) / 2
                    bbox.center.y = (box.xyxy[1] + box.xyxy[3]) / 2
                    bbox.size_x = box.xyxy[2] - box.xyxy[0]
                    bbox.size_y = box.xyxy[3] - box.xyxy[1]
                    detection.bbox = bbox
                    detection_array.detections.append(detection)

            self.publisher_.publish(detection_array)

def main(args=None):
    rclpy.init(args=args)
    node = ObjectDetectionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
