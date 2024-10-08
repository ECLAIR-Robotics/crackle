import struct

import numpy as np
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2

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
        print("point_cloud_array:", point_cloud_array)
        # pcd.points = o3d.utility.Vector3dVector(point_cloud_array)
        # self.publish_point_cloud(pcd, msg)


    
    def extract_point_cloud_array(self, msg : PointCloud2):
        """
        header: std_msgs.msg.Header(stamp=builtin_interfaces.msg.Time(sec=1728346425, nanosec=622569336), frame_id='camera_depth_optical_frame')
        fields: [sensor_msgs.msg.PointField(name='x', offset=0, datatype=7, count=1), sensor_msgs.msg.PointField(name='y', offset=4, datatype=7, count=1), sensor_msgs.msg.PointField(name='z', offset=8, datatype=7, count=1), sensor_msgs.msg.PointField(name='rgb', offset=16, datatype=7, count=1)]
        height: 1
        width: 117229
        point step: 20
        row step: 2344580
        """
        point_cloud_array: list = []
        # print("header:", msg.header)
        # print("fields:",msg.fields)
        # print("height:",msg.height)
        # print("width:",msg.width)
        # print("point step:",msg.point_step)
        # print("row step:",msg.row_step)

        print(msg.data)
            
        return np.asarray(point_cloud_array)
    
    def publish_point_cloud(self, pcd, input_msg : PointCloud2):
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