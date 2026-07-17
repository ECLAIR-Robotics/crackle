from typing import List, Tuple
import os
import math
import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup, MutuallyExclusiveCallbackGroup
from ament_index_python import get_package_share_directory
from geometry_msgs.msg import Pose, Point
from geometry_msgs.msg import Pose, Point

from std_srvs.srv import Trigger, Empty
from sensor_msgs.msg import Image, PointCloud2, CameraInfo
from crackle_interfaces.srv import FindObjects
from shape_msgs.msg import SolidPrimitive, Mesh, MeshTriangle
from shape_msgs.msg import SolidPrimitive, Mesh, MeshTriangle
from crackle_vision.pcd_tools import get_best_approx # @ Shalini
from moveit_msgs.msg import CollisionObject
import cv2
import matplotlib.pyplot as plt
from cv_bridge import CvBridge
import numpy as np
from ultralytics import YOLOE, YOLO
from image_geometry import PinholeCameraModel
import open3d as o3d
import re
from tf2_ros import Buffer, TransformListener
from scipy.spatial.transform import Rotation
from sensor_msgs_py import point_cloud2
from std_msgs.msg import Header


# Pixels to erode off the segmentation mask before back-projecting to a point
# cloud. The RealSense's aligned depth "bleeds" across an object's silhouette:
# the 1–few px ring at the mask border picks up mixed foreground/table depth and
# back-projects into a skirt of points on the table that inflates the fitted
# collision box. Eroding the mask by a few px drops that ring. Too large and we
# start eating real object; ~4 px is a good balance at typical tabletop range.
MASK_ERODE_PX = 4


def sanitize_moveit_id(name: str) -> str:
    """Convert an object name into a safe MoveIt collision-object id.

    YOLO class labels can contain spaces (e.g. "cell phone", "wine glass"),
    which cause problems downstream when used as collision-object ids. Collapse
    any whitespace to single underscores so every id we add to MoveIt is
    underscore-delimited.
    """
    return re.sub(r"\s+", "_", name.strip())


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

        self.person_dictionary = {}

        self.bridge = CvBridge()
        self.model = YOLOE(os.path.join(VisionServerNode.DATA_DIRECTORY, "yoloe-11l-seg-pf.pt"))
        self.model_prompt = YOLOE(os.path.join(VisionServerNode.DATA_DIRECTORY, "yoloe-11l-seg.pt"))

        self.camera_link = "camera_depth_optical_frame"
        self.link_base = "link_base"

        # TF so we know which way is up (gravity) in the camera frame — needed to
        # fit upright, table-resting object boxes regardless of the camera angle.
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # Debug: the exact point cloud each object's collision box is fit to.
        # Add this topic as a PointCloud2 in rviz (fixed frame = camera frame) to
        # see what the approximation actually uses.
        self.fit_cloud_pub = self.create_publisher(
            PointCloud2, "vision/object_fit_cloud", 10
        )

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
            Empty,
            "vision/scan_objects",
            self.scan_objects,
            callback_group=self._service_callback_group,
        )

        self.create_service(
            FindObjects,
            "vision/find_objects",
            self.find_objects_service_callback,
            callback_group=self._service_callback_group,
        )

        self.create_service(
            Trigger,
            "vision/scan",
            self.scan_objects,
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

    def publish_fit_cloud(self, o3d_cloud):
        """Publish the point cloud used to fit an object's collision box, so it
        can be visualized in rviz against the resulting collision object."""
        if o3d_cloud is None:
            return
        try:
            pts = np.asarray(o3d_cloud.points, dtype=np.float32)
            if pts.size == 0:
                return
            header = Header()
            header.stamp = self.get_clock().now().to_msg()
            header.frame_id = self.camera_link
            msg = point_cloud2.create_cloud_xyz32(header, pts.tolist())
            self.fit_cloud_pub.publish(msg)
            self.get_logger().info(
                f"Published {len(pts)} fit-cloud points to vision/object_fit_cloud"
            )
        except Exception as e:
            self.get_logger().warn(f"Failed to publish fit cloud: {e}")

    def gravity_up_in_camera(self):
        """World/base +Z ('up') as a unit vector in the camera optical frame.

        Used to fit upright object boxes. Returns None if the camera->link_base
        transform isn't available yet, in which case shape fitting falls back to
        the view-axis method.
        """
        try:
            tf = self.tf_buffer.lookup_transform(
                self.camera_link, self.link_base, rclpy.time.Time()
            )
        except Exception as e:
            self.get_logger().warn(
                f"gravity TF ({self.camera_link}<-{self.link_base}) unavailable: {e}"
            )
            return None
        q = tf.transform.rotation
        # This rotation maps link_base vectors into the camera frame; its third
        # column is link_base's +Z (up) expressed in the camera frame.
        rot = Rotation.from_quat([q.x, q.y, q.z, q.w]).as_matrix()
        up = np.asarray(rot)[:, 2]
        return up / (np.linalg.norm(up) + 1e-12)

    def strip_support_plane(self, pcd, up):
        """Remove the horizontal table surface from an object cloud.

        Even after mask erosion, a thin skirt of tabletop points can survive
        around the object base. RANSAC-fit the dominant plane; if it is roughly
        horizontal (its normal aligns with gravity ``up``) drop only the inliers
        sitting in the BOTTOM height band — the table level — so the skirt is
        removed while the object's own surfaces, including a horizontal top face
        seen top-down (which lives at HIGH height), are preserved. Returns the
        cloud unchanged if ``up`` is unavailable or the plane isn't a table.
        """
        try:
            n = len(pcd.points)
            if up is None or n < 40:
                return pcd
            up = np.asarray(up, dtype=float)
            up = up / (np.linalg.norm(up) + 1e-12)

            plane_model, inliers = pcd.segment_plane(
                distance_threshold=0.008, ransac_n=3, num_iterations=200
            )
            if len(inliers) < 0.15 * n:
                return pcd  # no dominant plane — nothing table-like to strip

            normal = np.asarray(plane_model[:3], dtype=float)
            normal = normal / (np.linalg.norm(normal) + 1e-12)
            if abs(float(np.dot(normal, up))) < 0.85:
                return pcd  # a vertical/skew plane (e.g. an object face) — keep it

            heights = np.asarray(pcd.points) @ up
            bottom_band = np.percentile(heights, 25.0)
            inlier_mask = np.zeros(n, dtype=bool)
            inlier_mask[np.asarray(inliers)] = True
            # Drop only the plane points at/below the bottom band (the table),
            # never higher object surfaces that happen to be coplanar-ish.
            drop = inlier_mask & (heights <= bottom_band)
            kept = pcd.select_by_index(np.where(~drop)[0])
            if len(kept.points) < 20:
                return pcd
            return kept
        except Exception as e:
            self.get_logger().warn(f"support-plane strip failed: {e}")
            return pcd

    def depth_to_pcd(self, mask : np.array) -> o3d.geometry.PointCloud:

        # convert depth image to point cloud
        if self.depth_image is None:
            self.get_logger().info("No depth image available")
            return None

        if self.intrinsic_matrix is None:
            return

        depth_image = self.bridge.imgmsg_to_cv2(self.depth_image, desired_encoding="passthrough")

        # Erode the mask to strip the silhouette border where aligned depth bleeds
        # from the object onto the table behind it (see MASK_ERODE_PX). Those
        # mixed-depth edge points otherwise form a skirt that oversizes the box.
        mask_u8 = mask.astype(np.uint8)
        if MASK_ERODE_PX > 0:
            kernel = np.ones((3, 3), np.uint8)
            mask_u8 = cv2.erode(mask_u8, kernel, iterations=MASK_ERODE_PX)

        depth_image = cv2.bitwise_and(depth_image, depth_image, mask=mask_u8)
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

    def scan_objects(self, request, response) -> Tuple[List[str], List[SolidPrimitive]]:
        """Detect objects in the current color image, filter by confidence, and parallelize mesh computation.
        """
        # Removed multiprocessing for simpler, sequential execution

        if self.color_image is None or self.depth_image is None:
            self.get_logger().info("No images available for find_objects")
            return [], []
        names = None
        min_confidence = 0.67
        boxes, segments, class_names = self.get_object_bounding_boxes(names if names else [])
        if boxes is None or segments == []:
            return [], []

        try:
            confs = boxes.conf.cpu().numpy()
        except Exception:
            confs = np.ones(len(class_names), dtype=float)

        # Prepare jobs for parallel mesh computation
        jobs = []
        job_names = []
        for idx, cls_name in enumerate(class_names):
            if names and cls_name not in names:
                continue
            if confs[idx] < min_confidence:
                continue
            try:
                segment_mask = segments[idx].cpu()
                segment_mask = segment_mask.data.numpy()[0]
            except Exception:
                self.get_logger().warning(f"Could not get segment mask for index {idx}")
                continue
            pcd = self.depth_to_pcd(segment_mask)
            if pcd is None:
                self.get_logger().warning(f"No point cloud available for {cls_name}; skipping")
                continue
            try:
                pcd = pcd.voxel_down_sample(voxel_size=0.005)
                pcd, ind = pcd.remove_statistical_outlier(nb_neighbors=20, std_ratio=2.0)
            except Exception as e:
                self.get_logger().warning(f"Point cloud preprocessing failed for {cls_name}: {e}")
            with o3d.utility.VerbosityContextManager(o3d.utility.VerbosityLevel.Error) as cm:
                labels = np.array(pcd.cluster_dbscan(eps=0.02, min_points=10, print_progress=False))
            if labels.size == 0:
                self.get_logger().warning(f"No clusters found for {cls_name}")
                continue
            unique_labels, counts = np.unique(labels, return_counts=True)
            valid = [(lab, cnt) for lab, cnt in zip(unique_labels, counts) if lab != -1]
            if not valid:
                self.get_logger().warning(f"Only noise detected for {cls_name}")
                continue
            largest_cluster_label = max(valid, key=lambda x: x[1])[0]
            indices = np.where(labels == largest_cluster_label)[0]
            largest_cluster = pcd.select_by_index(indices)
            # Strip any residual tabletop skirt so the box fits the object.
            largest_cluster = self.strip_support_plane(
                largest_cluster, self.gravity_up_in_camera())
            # Serialize point cloud for multiprocessing (as numpy array)
            jobs.append(np.asarray(largest_cluster.points))
            job_names.append(cls_name)

        # Helper for multiprocessing: must be top-level or staticmethod
        def mesh_worker(points):
            import open3d as o3d
            from crackle_vision.pcd_tools import get_best_approx
            pcd = o3d.geometry.PointCloud()
            pcd.points = o3d.utility.Vector3dVector(points)
            try:
                shape_mesh, shape_type, quaternion, fit_cloud = get_best_approx(pcd, up=self.gravity_up_in_camera())
                self.publish_fit_cloud(fit_cloud)
                return (shape_mesh, shape_type, quaternion)
            except Exception as e:
                return None

        # Run mesh_worker sequentially for up to 4 jobs (no multiprocessing)
        mesh_results = []
        for idx, points in enumerate(jobs):
            if idx >= 4:
                break
            try:
                mesh_results.append(mesh_worker(points))
            except Exception as e:
                self.get_logger().warning(f"mesh_worker failed for job {idx}: {e}")

        detected_names: List[str] = []
        primitives: List[SolidPrimitive] = []
        for idx, result in enumerate(mesh_results):
            if result is None:
                self.get_logger().warning(f"Shape approximation failed for {job_names[idx]}")
                continue
            shape_mesh, shape_type, quaternion = result
            object_solid_primitive = SolidPrimitive()
            object_solid_primitive.type = shape_type
            min_bound, max_bound = shape_mesh.get_min_bound(), shape_mesh.get_max_bound()
            # ! Don't set primitive dimensions because we are instead adding the mesh. Otherwise CollisionObject is published with two overlapping boxes.
            pose = Pose()
            pose.position.x = float((max_bound[0] + min_bound[0]) / 2)
            pose.position.y = float((max_bound[1] + min_bound[1]) / 2)
            pose.position.z = float((max_bound[2] + min_bound[2]) / 2)

            collision_object = CollisionObject()
            collision_object.header.frame_id = self.camera_link
            object_id = sanitize_moveit_id(job_names[idx])
            collision_object.id = object_id
            collision_object.primitives.append(object_solid_primitive)
            collision_object.primitive_poses.append(pose)

            collision_object_mesh = Mesh()
            for vertex in np.asarray(shape_mesh.vertices):    # shape_mesh is an o3d.TriangleMesh
                point = Point()
                point.x, point.y, point.z = vertex
                collision_object_mesh.vertices.append(point)
            for tri in np.asarray(shape_mesh.triangles):
                meshTri = MeshTriangle()
                meshTri.vertex_indices = tri.astype(np.uint32)    # assigning np.ndarray to uint32[3] --> will it work?
                collision_object_mesh.triangles.append(meshTri)

            collision_object.meshes.append(collision_object_mesh)

            collision_object.operation = CollisionObject.ADD
            self.collision_object_pub.publish(collision_object)
            detected_names.append(object_id)
            primitives.append(object_solid_primitive)
        # return detected_names, primitives
        trigger_response = Trigger.Response()
        self.get_logger().info(f"Detected objects: {detected_names}")
        return trigger_response


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

                        # Strip any residual tabletop skirt so the box fits the
                        # object, not the surface it rests on.
                        up_cam = self.gravity_up_in_camera()
                        pcd = self.strip_support_plane(pcd, up_cam)

                        shape_mesh, shape_type, quaternion, fit_cloud = get_best_approx(pcd, up=up_cam)
                        self.publish_fit_cloud(fit_cloud)
                        if shape_type == SolidPrimitive.BOX:
                            self.get_logger().info(f"Object {name} approximated as a box")
                        elif shape_type == SolidPrimitive.SPHERE:
                            self.get_logger().info(f"Object {name} approximated as a sphere")

                        # o3d.visualization.draw_geometries([shape_mesh])
                        object_solid_primitive = SolidPrimitive()
                        object_solid_primitive.type = shape_type
                        # ! Don't set dimensions because we are instead adding the mesh  
                        pose = Pose()
                        pose.position.x = (shape_mesh.get_max_bound()[0] + shape_mesh.get_min_bound()[0]) / 2
                        pose.position.y = (shape_mesh.get_max_bound()[1] + shape_mesh.get_min_bound()[1]) / 2
                        pose.position.z = (shape_mesh.get_max_bound()[2] + shape_mesh.get_min_bound()[2]) / 2

                        vertices = np.asarray(shape_mesh.vertices)
                        # offset all the vertices back by the pose.position

                        vertices[:,:] -= np.array([pose.position.x, pose.position.y, pose.position.z])
                        shape_mesh.vertices = o3d.utility.Vector3dVector(vertices)

                        print("Object position:", pose.position)
                        output.append(object_solid_primitive)

                        # Construct collision object and publish to MoveIt (Untested)
                        collision_object = CollisionObject()
                        collision_object.header.frame_id = "camera_depth_optical_frame"
                        collision_object.id = name
                        collision_object.pose = pose # ! checking if this messes up the position of the mesh --> it does not, after we set mesh vertex coordinates to be relative to pose position (object center).
                        collision_object.primitives.append(object_solid_primitive)
                        collision_object.primitive_poses.append(pose) # <-- When we tested the mesh method, this was commented --> there was no primitive pose?

                        # INSTEAD of setting the 'orientation' attribute of the Pose being assigned to the CollisionObject we will instead 
                        # just specify a position and assign the approximation-shape's mesh data to the 'meshes' attribute
                        # --> Do we still need the SolidPrimitive attribute of the CollisionObject (Just specifies dimensions of the Box)?
                        collision_object_mesh = Mesh()
                        for vertex in np.asarray(shape_mesh.vertices):    # shape_mesh is an o3d.TriangleMesh
                            point = Point()
                            point.x, point.y, point.z = vertex
                            collision_object_mesh.vertices.append(point)
                        for tri in np.asarray(shape_mesh.triangles):
                            meshTri = MeshTriangle()
                            meshTri.vertex_indices = tri.astype(np.uint32)    # assigning np.ndarray to uint32[3] --> will it work?
                            collision_object_mesh.triangles.append(meshTri)
                        
                        collision_object.meshes.append(collision_object_mesh)

                        collision_object.operation = CollisionObject.ADD
                        print("Collision object finalized.")
                        print("Collision object finalized.")
                        self.collision_object_pub.publish(collision_object)
                        print("Publish request sent.")
                        print("Publish request sent.")

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
