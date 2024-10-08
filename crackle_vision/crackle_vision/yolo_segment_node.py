from ultralytics import YOLO
from sensor_msgs.msg import Image
import rclpy
from rclpy.node import Node
from cv_bridge import CvBridge
import numpy as np
from std_msgs.msg import String
from crackle_interfaces.msg import YoloInferences, YoloSegment


class YoloSegmentNode(Node):

    def __init__(self):
        super().__init__("yolo_segment_node")
        self.model = YOLO("yolov8n-seg.pt")
        self.bridge = CvBridge()
        self.subscription = self.create_subscription(
            Image, "/camera/camera/color/image_raw", self.image_callback, 10
        )
        self.publisher = self.create_publisher(Image, "yolo_segmented_image", 10)
        self.classes_publisher = self.create_publisher(
            YoloInferences, "yolo_segmented_classes", 10
        )
        self.get_logger().info("YoloSegmentNode initialized.")

    def image_callback(self, msg):
        cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        results = self.model(cv_image)
        # Create an empty image to combine the segmented masks
        result = results[0]
        if result is None:
            return None
        # all classes that can be outputted by the yolo model
        if result.masks is None:
            return None
        segments = result.masks.cpu().xy
        if segments is None:
            return
        list_yolo_segments = []
        for segment in segments:
            yolo_segment = YoloSegment()
            yolo_segment.segment = segment.flatten().tolist()
            list_yolo_segments.append(yolo_segment)
        yolo_inference = YoloInferences()
        result_boxes_classes = result.boxes.cpu().numpy().cls
        classes_inferred = [result.names[i] for i in result_boxes_classes]
        yolo_inference.classes = classes_inferred
        yolo_inference.segments = list_yolo_segments
        self.classes_publisher.publish(yolo_inference)
        # print("Probabilities(Test): ", result.boxes.cpu().numpy())
        vis_frame = result.plot()

        # Convert the combined mask to image message
        image_message = self.bridge.cv2_to_imgmsg(vis_frame, encoding="bgr8")
        self.publisher.publish(image_message)


def main():
    rclpy.init()
    yolo_segment_node = YoloSegmentNode()
    rclpy.spin(yolo_segment_node)
    yolo_segment_node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
