from ultralytics import YOLO
from sensor_msgs.msg import Image
import rclpy
from rclpy.node import Node
from cv_bridge import CvBridge
import cv2
import numpy as np
from std_msgs.msg import String


class YoloSegmentNode(Node):

    def __init__(self):
        super().__init__("yolo_segment_node")
        self.model = YOLO("yolov8n-seg.pt")
        self.bridge = CvBridge()
        self.subscription = self.create_subscription(
            Image, "camera/image_raw", self.image_callback, 10
        )
        self.publisher = self.create_publisher(Image, "yolo_segmented_image", 10)
        self.classes_publisher = self.create_publisher(
            String, "yolo_segmented_classes", 10
        )
        self.get_logger().info("YoloSegmentNode initialized.")

    def image_callback(self, msg):
        cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        results = self.model(cv_image)
        # Create an empty image to combine the segmented masks
        combined_mask = np.zeros_like(cv_image)
        result = results[0]
        vis_frame = result.plot()
        # # Iterate over each segment mask
        # for i, result in enumerate(results):
        #     # Copy tensor to CPU
        #     mask = result.masks.data[0].numpy()

        #     # Convert mask to color image
        #     mask_image = (mask * 255).astype("uint8")
        #     mask_color = cv2.cvtColor(mask_image, cv2.COLOR_GRAY2BGR)

        #     # Generate a random color for each segment
        #     color = np.random.randint(0, 255, size=(3,), dtype=np.uint8)

        #     # Apply color to the segment mask
        #     colored_mask = cv2.bitwise_and(mask_color, (color[0], color[1], color[2]))

        #     # Combine the colored mask with the combined mask
        #     combined_mask = cv2.bitwise_or(combined_mask, colored_mask)

        # Convert the combined mask to image message
        image_message = self.bridge.cv2_to_imgmsg(vis_frame, encoding="bgr8")
        self.publisher.publish(image_message)
        self.get_logger().info("Image received.")


def main():
    rclpy.init()
    yolo_segment_node = YoloSegmentNode()
    rclpy.spin(yolo_segment_node)
    yolo_segment_node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
