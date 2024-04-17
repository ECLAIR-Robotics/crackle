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
        # Run inference and display results
        results = self.model(source=0, show=True, conf=0.4, save=True)

        # Calculate and display the inference time
        end_time = time.time()
        print(f"Inference time: {end_time - start_time:.2f} seconds")

        # Accessing and printing specific metrics like precision or recall if needed
        # This assumes model has been validated and these metrics are available
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




