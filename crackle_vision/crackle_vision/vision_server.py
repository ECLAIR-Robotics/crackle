from typing import List, Tuple
import os
import math
import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup, MutuallyExclusiveCallbackGroup
from ament_index_python import get_package_share_directory
from geometry_msgs.msg import Pose

from std_srvs.srv import Empty
from sensor_msgs.msg import Image, PointCloud2, CameraInfo
from crackle_interfaces.srv import FindObjects
from shape_msgs.msg import SolidPrimitive
from crackle_vision.pcd_tools import get_best_approx # @ Shalini
from moveit_msgs.msg import CollisionObject
import cv2
import matplotlib.pyplot as plt
from cv_bridge import CvBridge
import numpy as np
from ultralytics import YOLOE, YOLO
from image_geometry import PinholeCameraModel
import open3d as o3d

class VisionServerNode(Node):

    DATA_DIRECTORY = os.path.join(get_package_share_directory("crackle_vision"),"data")

    def __init__(self, node_name):
        super().__init__(node_name)
        self._vision_callback_group = ReentrantCallbackGroup()
        self._service_callback_group = MutuallyExclusiveCallbackGroup()
        self.depth_image: Image = None
        self.color_image: Image = None
        self.pcd: PointCloud2 = None
        self.intrinsic_matrix = None
        self.depth_instrinsic_matrix = None
        self.camera_model = PinholeCameraModel()

        self.bridge = CvBridge()
        self.model = YOLOE(os.path.join(VisionServerNode.DATA_DIRECTORY, "yoloe-11l-seg-pf.pt"))
        self.model_prompt = YOLOE(os.path.join(VisionServerNode.DATA_DIRECTORY, "yoloe-11l-seg.pt"))

        self.camera_link = "camera_depth_optical_frame"
        self.link_base = "link_base"

        self.class_names = []
        self.segments = []

        self.initial_depth = True
        self.initial_pcd = True

        self.depth_image_sub = self.create_subscription(
            Image,
            "/camera/camera/aligned_depth_to_color/image_raw",
            self.depth_image_callback,
            1,
            callback_group=self._vision_callback_group,
        )

        self.color_image_sub = self.create_subscription(
            Image,
            "/camera/camera/color/image_raw",
            self.color_image_callback,
            1,
            callback_group=self._vision_callback_group,
        )

        self.pcd_sub = self.create_subscription(
            PointCloud2,
            "/camera/camera/depth/color/points",
            self.pcd_callback,
            1,
            callback_group=self._vision_callback_group,
        )


        self.info_sub = self.create_subscription(
            CameraInfo,
            "camera/camera/aligned_depth_to_color/camera_info",
            self.set_depth_intrinsic_matrix,
            1,
            callback_group=self._vision_callback_group,
        )

        self.create_service(
            FindObjects,
            "vision/find_objects",
            self.find_objects_service_callback,
            callback_group=self._service_callback_group,
        )

        self.create_service(
            Empty,
            "vision/pcd",
            self.depth_to_pcd,
            callback_group=self._service_callback_group,
        )

        self.plot_pub = self.create_publisher(
            Image,
            "vision/results/image_raw",
            10,
        )

        self.collision_object_pub = self.create_publisher(
            CollisionObject,
            "crackle_moveit/publish_collision_object",
            10
        )

    def set_camera_model(self, msg: CameraInfo):
        """Sets the camera model based on the CameraInfo message.

        Args:
            msg: The CameraInfo message.
        """
        self.camera_model.fromCameraInfo(msg)

    def set_color_intrinsic_matrix(self, msg: CameraInfo):
        """Sets the intrinsic matrix for the color camera.

        Args:
            msg: The CameraInfo message.
        """
        self.set_camera_model(msg)
        self.intrinsic_matrix = [
            [msg.k[0], msg.k[1], msg.k[2]],
            [msg.k[3], msg.k[4], msg.k[5]],
            [msg.k[6], msg.k[7], msg.k[8]],
        ]

    def set_depth_intrinsic_matrix(self, msg: CameraInfo):
        """Sets the intrinsic matrix for the depth camera.

        Args:
            msg: The CameraInfo message.
        """
        self.set_camera_model(msg)
        self.intrinsic_matrix = [
            [msg.k[0], msg.k[1], msg.k[2]],
            [msg.k[3], msg.k[4], msg.k[5]],
            [msg.k[6], msg.k[7], msg.k[8]],
        ]

    def depth_image_callback(self, msg):
        self.depth_image = msg


    def color_image_callback(self, msg):
        self.color_image = msg

    def pcd_callback(self, msg):
        self.pcd = msg

    def depth_to_pcd(self, mask : np.array) -> o3d.geometry.PointCloud:

        # convert depth image to point cloud
        if self.depth_image is None:
            self.get_logger().info("No depth image available")
            return None

        if self.intrinsic_matrix is None:
            return


        depth_image = self.bridge.imgmsg_to_cv2(self.depth_image, desired_encoding="passthrough")

        depth_image = cv2.bitwise_and(depth_image, depth_image, mask=mask.astype(np.uint8))
        points_arr = []
        inv_camera_matrix = np.linalg.inv(self.intrinsic_matrix)
        for row, col in np.ndindex(depth_image.shape):
            z = depth_image[row, col] * 0.001
            point_np = np.array([col, row, 1]) 
            point_transformed = np.dot(inv_camera_matrix, point_np)
            point_scaled = point_transformed * z 
            points_arr.append(np.array([point_scaled[0], point_scaled[1], point_scaled[2]]))
        points_arr = np.array(points_arr)

        # Convert to Open3D point cloud
        pcd = o3d.geometry.PointCloud()   
        pcd.points = o3d.utility.Vector3dVector(points_arr)
        print(pcd)
        return pcd


    def find_objects_service_callback(self, request, response):
        """Handles the FindObjects service request.

        Callback for the vision/find_objects service. This service finds all the objects in
        the request list and returns their solid geometry

        Args:
            request: The FindObjects service request.
            response: The FindObjects service response.

        Returns:
            A list of booleans mapped to the object names; True if object was found False if not .
        """

        self.get_logger().info("Received FindObjects request")
        if self.color_image is not None and self.depth_image is not None:
            boxes, segments, names = self.get_object_bounding_boxes(request.names)
            if boxes is None or segments == []:
                self.get_logger().info("No objects found")
                response.names = []
                response.objects = []
                return response

            print("names:", names)
            output = []
            for index, name in enumerate(names):
                if name in request.names:
                    segment_mask = segments[index].cpu()
                    segment_mask = segment_mask.data.numpy()[0]
                    color_image = self.bridge.imgmsg_to_cv2(self.color_image, desired_encoding="bgr8")
                    cv2_color_image = cv2.cvtColor(color_image, cv2.COLOR_BGR2RGB)

                    # apply segment mask to color image 
                    masked_image = cv2.bitwise_and(color_image, color_image, mask=segment_mask.astype(np.uint8))

                    pcd = self.depth_to_pcd(segment_mask)

                    if pcd is None:
                        self.get_logger().info(str(self.depth_image is None))
                        self.get_logger().info("No point cloud available")
                        response.names = []
                        response.objects = []
                    else:
                        
                        # downsample the point cloud
                        pcd = pcd.voxel_down_sample(voxel_size=0.005)

                        # remove statistical outliers
                        pcd, ind = pcd.remove_statistical_outlier(nb_neighbors=20, std_ratio=2.0)

                        # find clusters - labels each point.
                        with o3d.utility.VerbosityContextManager(
                                o3d.utility.VerbosityLevel.Debug) as cm:
                            labels = pcd.cluster_dbscan(eps=0.02, min_points=10, print_progress=True)

                        # get largest cluster (hopefully the closest cluster, i.e. object)
                        unique_labels, counts = np.unique(labels, return_counts=True)
                        largest_cluster_label = unique_labels[np.argmax(counts)]
                        largest_cluster = pcd.select_by_index(np.where(labels == largest_cluster_label)[0])
                        pcd = largest_cluster 

                        shape_mesh, shape_type, quaternion = get_best_approx(pcd)
                        if shape_type == SolidPrimitive.BOX:
                            self.get_logger().info(f"Object {name} approximated as a box")
                        elif shape_type == SolidPrimitive.SPHERE:
                            self.get_logger().info(f"Object {name} approximated as a sphere")

                        # o3d.visualization.draw_geometries([shape_mesh])
                        object_solid_primitive = SolidPrimitive()
                        object_solid_primitive.type = shape_type
                        print("Shape mesh bounds:", (shape_mesh.get_min_bound() - shape_mesh.get_max_bound()))
                        object_solid_primitive.dimensions = (shape_mesh.get_max_bound() - shape_mesh.get_min_bound()).tolist()
                        print("Object dimensions:", object_solid_primitive.dimensions)
                        pose = Pose()
                        pose.position.x = (shape_mesh.get_max_bound()[0] + shape_mesh.get_min_bound()[0]) / 2
                        pose.position.y = (shape_mesh.get_max_bound()[1] + shape_mesh.get_min_bound()[1]) / 2
                        pose.position.z = (shape_mesh.get_max_bound()[2] + shape_mesh.get_min_bound()[2]) / 2
                        pose.orientation.x = float(quaternion[0])
                        pose.orientation.y = float(quaternion[1])
                        pose.orientation.z = float(quaternion[2])
                        pose.orientation.w = float(quaternion[3])
                        print("Object position:", pose.position)
                        print("Object orientation:", pose.orientation)
                        # visualize the point cloud
                        # o3d.visualization.draw_geometries([pcd]) 
                        viewer = o3d.visualization.Visualizer()
                        viewer.create_window(window_name="Point Cloud", width=800, height=600)
                        opt = viewer.get_render_option()
                        opt.background_color = np.asarray([0, 0, 0])
                        opt.show_coordinate_frame = True
                        viewer.add_geometry(pcd)
                        viewer.run()
                        viewer.destroy_window()
                        output.append(object_solid_primitive)

                        # Construct collision object and publish to MoveIt (Untested)
                        collision_object = CollisionObject()
                        collision_object.header.frame_id = "camera_depth_optical_frame"
                        collision_object.id = name
                        collision_object.primitives.append(object_solid_primitive)
                        collision_object.primitive_poses.append(pose)
                        collision_object.operation = CollisionObject.ADD
                        self.collision_object_pub.publish(collision_object)

            response.names = names
            response.objects = output

        else:
            self.get_logger().info("No Image available")

        return response

    def get_object_bounding_boxes(self, names : List[str] = []) -> Tuple[List, List, List]:
        """Gets the bounding boxes of specified objects.

        Currently returns an empty list as a placeholder.

        Args:
            names: A list of object names.

        Returns:
            A list of bounding boxes.
        """
        self.get_logger().info("Getting object bounding boxes")
        cv_image = self.bridge.imgmsg_to_cv2(self.color_image, desired_encoding="bgr8")

        model = None
        if (len(names) > 0):
            model = self.model_prompt
            model.set_classes(names, model.get_text_pe(names))
        else:
            model = self.model

        # Execute prediction for specified categories on an image
        results = model.predict(cv_image)
        result = results[0]
        if result is None or result.boxes is None or result.masks is None:
            self.get_logger().info("No objects found")
            return [], [], []
        
        self.get_logger().info(f"Found {len(result.boxes)} objects")

        boxes = result.boxes.cpu()
        segments = result.masks.cpu()

        class_ids = boxes.cls.cpu().numpy()
        self.class_names = [result.names[int(cls)] for cls in class_ids]
        self.segments = segments 

        result_plot = result.plot()
        print("Result plot shape:", result_plot.shape)
        self.plot_pub.publish(self.bridge.cv2_to_imgmsg(result_plot, encoding="bgr8"))
        return boxes, segments, self.class_names


def main():
    rclpy.init()
    executor = MultiThreadedExecutor(num_threads=4)
    vision_node = VisionServerNode("vision_server_node")
    executor.add_node(vision_node)
    vision_node.get_logger().info("Vision server node started")
    executor.spin()  # This will block until the node is shut down
    executor.shutdown()  # Shutdown the executor
    vision_node.destroy_node()
    rclpy.shutdown()  # Shutdown the rclpy library


if __name__ == "__main__":
    main()
