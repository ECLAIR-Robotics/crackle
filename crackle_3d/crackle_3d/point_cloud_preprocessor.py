import struct

import numpy as np
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
import ros2_numpy as rnp
import open3d as o3d

class PointCloudPreprocessorNode(Node):

    def __init__(self):
        super().__init__("point_cloud_preprocessor_node")
        self.subscription = self.create_subscription(
            PointCloud2, "/camera/camera/depth/color/points", self.point_cloud_callback, 10
        )
        # self.filtered_pcd_publisher = self.create_publisher(PointCloud2, "/crackle_3d/filtered_cloud", 10)
        self.get_logger().info("PointCloudPreprocessorNode initialized.")

    def point_cloud_callback(self, msg : PointCloud2):
        pcd = o3d.geometry.PointCloud()
        point_cloud_array = self.extract_point_cloud_array(msg)
        point_cloud_array_xyz = point_cloud_array['xyz']
        print(type(point_cloud_array))
        print("point_cloud_array:", point_cloud_array_xyz)
        pcd.points = o3d.utility.Vector3dVector(point_cloud_array_xyz)
        o3d.visualization.draw_geometries([pcd])
        # self.publish_point_cloud(pcd, msg)


    
    def extract_point_cloud_array(self, msg : PointCloud2):
        """
        Extracts a point cloud array from a PointCloud2 message.

        Args:
            msg (PointCloud2): The PointCloud2 message containing the point cloud data.
        Returns:
            np.array: A numpy array representation of the point cloud data.

        """
        point_cloud_array: list = []
        print("header:", msg.header)
        print("fields:",msg.fields)
        print("height:",msg.height)
        print("width:",msg.width)
        print("point step:",msg.point_step)
        print("row step:",msg.row_step)

        array: np.array = rnp.numpify(msg)
        
            
        return array
    
    def publish_point_cloud(self, pcd, input_msg : PointCloud2) -> np.array :
        """
        Publishes the point cloud data to the filtered_pcd_publisher.

        Args:
            pcd: The point cloud data to be published.

            input_msg (PointCloud2): The input PointCloud2 message containing the header and fields information.

        Returns:
            np.array: The array representation of the point cloud points.
        """
        points = np.asarray(pcd.points)
        new_data = bytearray()
        output_msg = PointCloud2()
        output_msg.header = input_msg.header
        output_msg.height = 1
        output_msg.width = len(points)
        output_msg.fields = input_msg.fields
        output_msg.point_step = 16
        output_msg.row_step = 16 * len(points)
        output_msg.is_bigendian = input_msg.is_bigendian
        output_msg.is_dense = True
        for x, y, z in points:
            new_data += struct.pack("ffff", x / 100, y/100, z/100, i)
        output_msg.data = new_data
        self.filtered_pcd_publisher.publish(output_msg)



def main():
    rclpy.init()
    point_cloud_preprocessor_node = PointCloudPreprocessorNode()
    rclpy.spin(point_cloud_preprocessor_node)
    point_cloud_preprocessor_node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()