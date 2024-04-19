import rclpy
from rclpy.node import Node
from std_msgs.msg import String

from ultralytics import YOLO  # Assuming you are using YOLO from Ultralytics
import time

class ObjectDetectionNode(Node):
    def __init__(self):
        
        super().__init__('object_detection_node')
        self.publisher_ = self.create_publisher(String, 'detection_results', 10)
        self.model = YOLO('yolov8n.pt')  # Load the model
        self.timer = self.create_timer(0.1, self.run_detection)  # Adjust timer as needed

    def run_detection(self):
        start_time = time.time()
        # Run the model with JSON output enabled
        results = self.model(source=0, conf=0.4, save=True, export='json')
        end_time = time.time()

        # The results are now a JSON string, which includes bounding boxes, classes, and scores
        self.publisher_.publish(String(data=results.json()))

        print(f"Inference time: {end_time - start_time:.2f} seconds")

        if hasattr(results, 'metrics'):
            print(f"Precision: {results.metrics.precision}")
            print(f"Recall: {results.metrics.recall}")



def main(args=None):
    rclpy.init(args=args)
    node = ObjectDetectionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()




