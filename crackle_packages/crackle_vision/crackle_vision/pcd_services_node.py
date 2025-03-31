import rclpy
from rclpy.node import Node
from crackle_vision.pcd_tools import get_best_approx
from sensor_msgs.msg import PointCloud2

class PCDServicesNode(Node):
    def __init__(self):
        super().__init__('pcd_services_node')
        self.get_logger().info('PCD Services Node started')

        # Create services
        self.get_best_approx_service = self.create_service(
            PointCloud2,
            'get_best_approx',
            self.get_best_approx_callback
        )



def main(args=None):
    rclpy.init(args=args)
    pcd_services_node = PCDServicesNode()
    rclpy.spin(pcd_services_node)
    pcd_services_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()