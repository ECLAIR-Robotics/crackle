import struct

import numpy as np
import rclpy
from rclpy.node import Node
from typing import List
from sensor_msgs.msg import PointField
from sensor_msgs.msg import PointCloud2
from std_msgs.msg import Header
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
        self.pcd_header : Header = None
        self.height : int = None
        self.width : int = None 
        self.fields : List[PointField] = None 
        self.is_bigendian : bool = None
        self.point_step : int = None
        self.row_step : int = None


    def point_cloud_callback(self, msg : PointCloud2):
        self.pcd_header = msg.header
        self.height = msg.height
        self.width = msg.width 
        self.fields = msg.fields 
        self.is_bigendian =  msg.is_bigendian   
        self.point_step = msg.point_step
        self.row_step = msg.row_step

        pcd = o3d.geometry.PointCloud()
        point_cloud_array = rnp.numpify(msg)
        # point_cloud_array_xyz = point_cloud_array['xyz']
        print("point_cloud_array:", point_cloud_array)
        # print(type(point_cloud_array))
        # print("point_cloud_array:", point_cloud_array_xyz)
        # pcd.points = o3d.utility.Vector3dVnumpifyector(point_cloud_array_xyz)
        # o3d.visualization.draw_geometries([pcd])
        self.publish_point_cloud()

    
    def extract_point_cloud_array(self, msg : PointCloud2):
        """
        Extracts a point cloud array from a PointCloud2 message.

        Args:
            msg (PointCloud2): The PointCloud2 message containing the point cloud data.
        Returns:
            np.array: A numpy array representation of the point cloud data.

        """
        point_cloud_array: list = []
        print("header:", self.pcd_header)
        print("fields:",self.fields)
        print("height:",self.height)
        print("width:",self.width)
        print("point step:",self.point_step)
        print("row step:",self.row_step)
            
    
    def publish_point_cloud(self, pcd : o3d.geometry.PointCloud) -> np.array :
        """
        Publishes the point cloud data to the filtered_pcd_publisher.

        Args:
            pcd: The point cloud data to be published.

            input_msg (PointCloud2): The input PointCloud2 message containing the header and fields information.

        Returns:
            np.array: The array representation of the point cloud points.
        """
        self.get_logger().info("header:" + str(self.pcd_header.stamp))
        self.get_logger().info("fields:"+ str(self.fields))
        self.get_logger().info("height:"+str(self.height))
        self.get_logger().info("width:"+str(self.width))
        self.get_logger().info("point step:"+str(self.point_step))
        self.get_logger().info("row step:"+str(self.row_step))
        out_msg = PointCloud2() # @Shalini TODO
        out_msg.header = self.pcd_header
        out_msg.height = self.height
        out_msg.width = self.width
        out_msg.fields = self.fields
        out_msg.is_bigendian = self.is_bigendian
        out_msg.point_step = self.point_step
        out_msg.row_step = self.row_step

        out_msg.data = rnp.msgify(PointCloud2, pcd.points) 


def main():
    rclpy.init()
    point_cloud_preprocessor_node = PointCloudPreprocessorNode()
    rclpy.spin(point_cloud_preprocessor_node)
    point_cloud_preprocessor_node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()