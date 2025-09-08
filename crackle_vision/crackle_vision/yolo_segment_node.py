import math
import time
import cv2
from ultralytics import YOLO
from sensor_msgs.msg import Image
from sensor_msgs.msg import CameraInfo
import rclpy
from rclpy.node import Node
from cv_bridge import CvBridge
import numpy as np
from tf2_ros import TransformListener
from tf2_ros.buffer import Buffer
from crackle_interfaces.msg import YoloInferences, YoloSegment
from geometry_msgs.msg import PointStamped
from message_filters import ApproximateTimeSynchronizer, Subscriber
from visualization_msgs.msg import Marker
from sensor_msgs.msg import PointCloud2
import ros2_numpy as rnp
from scipy.spatial.transform import Rotation
import open3d as o3d
from xarm_msgs.srv import MoveCartesian, MoveJoint, Call, SetInt16, VacuumGripperCtrl, PlanExec, PlanPose, PlanJoint, PlanSingleStraight
from geometry_msgs.msg import Pose, PoseStamped
from moveit_msgs.msg import RobotState  
from crackle_vision.segment_img import get_object_points, get_center


class YoloSegmentNode(Node):


    def __init__(self):
        """ "

        - 636.9296875
        - 0.0
        - 644.2815551757812
        - 0.0
        - 636.9296875
        - 354.201416015625
        - 0.0
        - 0.0
        - 1.0

        """
        super().__init__("yolo_segment_node")
        self.CRACKLE_GRIPPER_OFFSET = 47
        self.model = YOLO("yolov8n.pt")
        self.bridge = CvBridge()
        self.camera_link = "camera_depth_optical_frame"
        self.link_base = "link_base"

        self.image_subscription = Subscriber(
            self, Image, "camera/camera/color/image_raw"
        )
        self.pcd_subscription = Subscriber(
            self, PointCloud2, "/camera/camera/depth/color/points"
        )

        self.camera_color_matrix_sub = self.create_subscription(CameraInfo,
             "/camera/camera/color/camera_info",self.set_color_intrinsic_matrix, 10)

        # self.camera_color_matrix_sub = self.create_subscription(CameraInfo,
             # "/camera/camera/color/camera_info",self.set_color_intrinsics, 10)
        self.camera_depth_matrix_sub = self.create_subscription(CameraInfo,
             "/camera/camera/depth/camera_info", self.set_depth_intrinsic_matrix, 10)

        self.point_marker_pub = self.create_publisher(Marker, "point_marker", 10)
        self.point_marker_pub_2 = self.create_publisher(Marker, "point_marker_2", 10)
        self.time_sync = ApproximateTimeSynchronizer(
            [self.image_subscription, self.pcd_subscription], 10, 0.05
        )
        self.publisher = self.create_publisher(Image, "yolo_segmented_image", 10)
        self.target_point = self.create_publisher(PointStamped, "target_point", 10)
        self.pcd_publisher = self.create_publisher(PointCloud2, "yolo_segment/pcd", 10)
        self.classes_publisher = self.create_publisher(
            YoloInferences, "yolo_segmented_classes", 10
        )
        self.time_sync.registerCallback(self.image_callback)
        self.intrinsic_matrix = [
            [607.619, 0.0, 317.497],
            [0.0, 607.511, 236.004],
            [0.0, 0.0, 1.0],
        ]
        self.depth_instrinsic_matrix = [
            [382.157, 0.0, 322.5689],
            [0.0, 382.157, 236.520],
            [0.0, 0.0, 1.0],
        ]

        # self._intrinsics = pyrealsense2.intrinsics()

        self.tranform_buffer = Buffer()
        self.tf_listener = TransformListener(self.tranform_buffer, self)
        self.arm_set_position_client = self.create_client(MoveCartesian, "/ufactory/set_position")
        self.arm_set_servo_angle_client = self.create_client(MoveJoint, "/ufactory/set_servo_angle")
        self.arm_clean_warning_client = self.create_client(Call, "/ufactory/clean_warn")
        self.arm_clean_error_client = self.create_client(Call, "/ufactory/clean_error")
        self.arm_set_mode_client = self.create_client(SetInt16, "/ufactory/set_mode")
        self.arm_set_state_client = self.create_client(SetInt16, "/ufactory/set_state")
        self.arm_set_vacuum_gripper_client = self.create_client(VacuumGripperCtrl, "/ufactory/set_vacuum_gripper")
        self.xarm_exec_plan = self.create_client(PlanExec, '/xarm_exec_plan')
        self.xarm_plan_pose = self.create_client(PlanPose, '/xarm_pose_plan')
        self.xarm_plan_joint = self.create_client(PlanJoint, '/xarm_joint_plan')
        self.xarm_plan_straight = self.create_client(PlanSingleStraight, '/xarm_straight_plan')

        # set mode to 0 
        set_mode_request = SetInt16.Request()
        set_mode_request.data = 0
        self.arm_set_mode_client.call_async(set_mode_request)

        # # set state to 0
        set_state_request = SetInt16.Request()
        set_state_request.data = 0
        self.arm_set_state_client.call_async(set_state_request)

        set_gripper_false = VacuumGripperCtrl.Request()
        set_gripper_false.on = False
        self.arm_set_vacuum_gripper_client.call_async(set_gripper_false)

        self.get_logger().info("YoloSegmentNode initialized.")

    def wait_for_service(self, client):
        while not client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...') 
    def plan_straight(self, pose : Pose):
        req = PlanPose.Request()
        req.target = pose
        future = self.xarm_plan_pose.call_async(req)
        print('Sent request')
        print("future: ", future.result())
        return future.result()


    def execute_done_callback(self, args):
        print('Execution done')

    def execute_plan(self):
        req = PlanExec.Request()
        future = self.xarm_exec_plan.call_async(req)
        future.add_done_callback(self.execute_done_callback)
        # rclpy.spin_until_future_complete(self, future)
        return future

    def set_color_intrinsic_matrix(self, msg: CameraInfo):
        self.intrinsic_matrix = [
            [msg.k[0], msg.k[1], msg.k[2]],
            [msg.k[3], msg.k[4], msg.k[5]],
            [msg.k[6], msg.k[7], msg.k[8]],
        ]
    
    def set_depth_intrinsic_matrix(self, msg: CameraInfo):
        self.depth_instrinsic_matrix = [
            [msg.k[0], msg.k[1], msg.k[2]],
            [msg.k[3], msg.k[4], msg.k[5]],
            [msg.k[6], msg.k[7], msg.k[8]],
        ]


    def send_request(self, x : float, y : float, z : float, roll : float, pitch : float, yaw : float, speed : float=0.0, acc: float=0.0, mvtime : float=0.0):
        """
        Send a request to move the arm to a specified position and orientation. 
        """
        pass

    def get_transform_buffer(self):
        try:
            
            transform = self.tranform_buffer.lookup_transform(
                self.link_base, self.camera_link, rclpy.time.Time()
            )
            
            translation = transform.transform.translation
            tx, ty, tz = translation.x, translation.y, translation.z
            rotation = transform.transform.rotation
            qx, qy, qz, qw = rotation.x, rotation.y, rotation.z, rotation.w

            r = Rotation.from_quat([qx, qy, qz, qw])
            R = r.as_matrix()
            T = np.eye(4)
            T[:3, :3] = R
            T[:3, 3] = [tx, ty, tz]
            print("Transform:", T)
            return T
        except Exception as e:
            self.get_logger().error(f"Error getting transform: {e}")
            return None

    def transform_point(self, point):
        T = self.get_transform_buffer()
        if T is None:
            return None
        point = np.array([point.x, point.y, point.z, 1])
        transformed_point = np.dot(T, point)
        print("Translation = ", T[:3, 3])
        return transformed_point[:3]
        # return T[:3, 3]

    def pointcloud2_to_o3d(self, msg):
        """
        Convert sensor_msgs/msg/PointCloud2 -> open3d.geometry.PointCloud
        - Supports fields: x,y,z and optional rgb OR r,g,b (uint8 or float in [0,1])
        - Filters out invalid (nan/inf) points
        """
        # Structured numpy array with named fields (x,y,z, rgb, etc.)
        arr = rnp.numpify(msg)  # shape: (H, W) for organized clouds, else (N,)

        # Stack XYZ into (H,W,3) or (N,3), then flatten to (N,3)
        xyz = np.stack([arr['x'], arr['y'], arr['z']], axis=-1).reshape(-1, 3)

        # Remove invalid points
        finite = np.isfinite(xyz).all(axis=1)
        xyz = xyz[finite].astype(np.float64, copy=False)

        # Build Open3D cloud
        pcd = o3d.geometry.PointCloud()
        pcd.points = o3d.utility.Vector3dVector(xyz)

        # ----- Optional colors -----
        colors = None
        fields = arr.dtype.names

        if 'rgb' in fields:
            # Common ROS packing: float32 storing bit-packed RGB
            # Get the underlying 32-bit integer without changing the bits
            rgb = arr['rgb'].reshape(-1)[finite]
            # Handle both float32-packed and uint32-packed cases
            if rgb.dtype == np.float32:
                rgb = rgb.view(np.uint32)
            elif rgb.dtype != np.uint32:
                rgb = rgb.astype(np.uint32, copy=False)

            r = ((rgb >> 16) & 255).astype(np.float32)
            g = ((rgb >> 8)  & 255).astype(np.float32)
            b = ( rgb        & 255).astype(np.float32)
            colors = np.stack([r, g, b], axis=1) / 255.0

        elif {'r','g','b'}.issubset(fields):
            r = arr['r'].reshape(-1)[finite].astype(np.float32)
            g = arr['g'].reshape(-1)[finite].astype(np.float32)
            b = arr['b'].reshape(-1)[finite].astype(np.float32)
            # If your r,g,b are 0..255 uint8, normalize; if already 0..1 floats this is a no-op
            if r.max() > 1.0 or g.max() > 1.0 or b.max() > 1.0:
                r /= 255.0; g /= 255.0; b /= 255.0
            colors = np.stack([r, g, b], axis=1).astype(np.float64, copy=False)

        elif 'intensity' in fields:
            # Grayscale from intensity if available
            inten = arr['intensity'].reshape(-1)[finite].astype(np.float32)
            # Normalize (robust) for visualization
            p1, p99 = np.percentile(inten, [1, 99]) if inten.size else (0, 1)
            denom = (p99 - p1) if (p99 - p1) > 1e-6 else 1.0
            gray = np.clip((inten - p1) / denom, 0, 1)
            colors = np.stack([gray, gray, gray], axis=1).astype(np.float64, copy=False)

        if colors is not None:
            pcd.colors = o3d.utility.Vector3dVector(colors)

        return pcd

    def image_callback(self, msg: Image, pcd: PointCloud2):
        """
            This callback executes when the image and point cloud messages are synchronized (published
            approximately at the same time.) It then gets bounding boxes from the image (YOLO) and then 
            calculates the x, y, z coordinates of the center of the bounding box in the point cloud.
        """

        print("Image callback triggered")
        pcd = self.pointcloud2_to_o3d(pcd)
        pcd_xyz = np.asarray(pcd.points)

        # classes_inferred, result_boxes_classes, segments = self.get_segment_results(msg)
        classes_inferred, result_boxes_classes, boxes = self.get_bounding_boxes(msg)

        print("Classes inferred:", classes_inferred)
        print("Result boxes classes:", result_boxes_classes)
        print("Boxes:", boxes)

        SEGMENT_NAME = "cell phone"

        # Check if a person is detected in the inferred classes
        if SEGMENT_NAME in classes_inferred:
            # person_index = classes_inferred.index("person")
            person_indices = [i for i, x in enumerate(classes_inferred) if x == SEGMENT_NAME]
            # segment_2_indices = [i for i, x in enumerate(classes_inferred) if x == SEGMENT_NAME2]

            person_confidences = [result_boxes_classes[index] for index in person_indices] 
            # segment_2_confidences = [result_boxes_classes[index] for index in segment_2_indices]

            max_confidence = max(person_confidences)
            # segment_2_max_confidence = max(segment_2_confidences)

            person_index = result_boxes_classes.tolist().index(max_confidence)
            # segment_2_index = result_boxes_classes.tolist().index(segment_2_max_confidence)

            # Get the segment points corresponding to the detected person
            segment_points = boxes[person_index]
            # segment_2_points = boxes[segment_2_index]

            print("Segment points:", segment_points)
            # print("Segment2 points:", segment_2_points)
            # Calculate the average x and y coordinates of the segment points

            # TODO FIX THIS
            segment_width = segment_points[2]
            segment_height = segment_points[3]
            average_x = float((segment_points[0] + segment_width) / 2)
            average_y = float((segment_points[1] + segment_height) / 2)

            # average_2_x = float(segment_2_points[0])
            # average_2_y = float(segment_2_points[1])

            print("Image x, y", average_x, average_y)

            instrinsic_matrix_inv = np.linalg.inv(self.intrinsic_matrix)

            print("Intrinsic Matrix Inv: ", instrinsic_matrix_inv)
            # Extract intrinsic camera parameters
            

            point_np = np.array([average_x, average_y, 1])
            point_transformed = np.dot(instrinsic_matrix_inv, point_np)
            print("Point transformed:", point_transformed)

            # Calculate distances from all points in the point cloud to the average point
            points_xy = pcd_xyz[:, :2]

            average_depth_x = float(point_transformed[0])
            average_depth_y = float(point_transformed[1])

            print("Points XY:", points_xy)
            distances = np.array([math.sqrt((x - average_depth_x) ** 2 + (y - average_depth_y) ** 2) for x, y in points_xy])

            print("distances: ", distances)
            # Find the index of the closest point in the point cloud
            min_index = np.argmin(distances)
            print("Min Index: ", min_index)
            closest_point = pcd_xyz[min_index]
            closest_point_z = closest_point[2]
            print("Average depth x:", average_depth_x)
            print("Average depth y:", average_depth_y)
            print("Closest point:", closest_point_z)
            # Create a PointStamped message for the closest point
            point_stamped: PointStamped = PointStamped()
            point_stamped.point.x = float(closest_point[0])
            point_stamped.point.y = float(closest_point[1])
            point_stamped.point.z = float(closest_point[2])
            point_stamped.header.frame_id = self.link_base

            

            # Transform the point to the world frame

            transformed_point = self.transform_point(point_stamped.point)
            print("Transformed point:", transformed_point)
            if transformed_point is None:
                transformed_point = [average_depth_x, average_depth_y, float(closest_point[2])]
            marker = Marker()
            marker.header.frame_id = "camera_depth_optical_frame"
            marker.header.stamp = self.get_clock().now().to_msg()
            marker.type = Marker.SPHERE
            marker.id = 0
            marker.pose.position.x = float(closest_point[0])
            marker.pose.position.y = float(closest_point[1])
            marker.pose.position.z = float(closest_point[2])
            marker.pose.orientation.x = 0.0
            marker.pose.orientation.y = 0.0
            marker.pose.orientation.z = 0.0
            marker.pose.orientation.w = 1.0
            marker.scale.x = 0.05
            marker.scale.y = 0.05
            marker.scale.z = 0.05
            marker.color.r = 1.0
            marker.color.a = 1.0
            self.point_marker_pub.publish(marker)

            # Publish the PointStamped message
            self.target_point.publish(point_stamped)
            
            MAX_Y_LIMIT = 150
            MIN_Y_LIMIT = -150
            MAX_X_LIMIT = 150
            MIN_X_LIMIT = -150  
            MAX_Z_LIMIT = 500
            MIN_Z_LIMIT = 500
            
            # transformed_point[0] =float(max(MIN_X_LIMIT, min(MAX_X_LIMIT, transformed_point[0] * 1000)))
            # transformed_point[1] =float(max(MIN_Y_LIMIT, min(MAX_Y_LIMIT, transformed_point[1] * 1000)))
            # transformed_point[2] =float(max(MIN_Z_LIMIT, min(MAX_Z_LIMIT, transformed_point[2] * 1000)))

            # Move the arm
            print("Sending request for point: ", transformed_point[0] * 1000.0, transformed_point[1] * 1000.0, transformed_point[2] * 1000.0)
            request = MoveCartesian.Request()
            
            tan_theta = transformed_point[1] / transformed_point[0]
            angle = np.arctan(tan_theta)
            request.pose = [transformed_point[0], transformed_point[1] , transformed_point[2] , 
                            float(-97), float(44.6), float(-87.1)]
            request.speed = 30.0
            request.acc = 15.0 
            request.mvtime = 1.0
            request.wait = True
            
            pose_request = Pose()
            pose_request.position.x = 218.0
            pose_request.position.y = 134.7
            pose_request.position.z = 328.6
            pose_request.orientation.x = float(88)
            pose_request.orientation.y = float(-40)
            pose_request.orientation.z = float(150)

            # self.plan_straight(pose_request)
            # time.sleep(1)
            # self.execute_plan()

            future = self.arm_set_position_client.call_async(request)
            rclpy.spin_until_future_complete(self, future)
            print(future.result())

            self.target_point.publish(point_stamped)
            # find the closest point to the average point in the pcd


    def get_bounding_boxes(self, msg: Image):
        """
            Pass in Ros2 Image message and get back yolo bounding boxes 
        """
        cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        results = self.model(cv_image)
        result = results[0]
        # # Create an empty image to combine the segmented masks
        if result is None:
            return None
        # # all classes that can be outputted by the yolo model
        boxes = result.boxes.cpu().xywh
        result_boxes_classes = result.boxes.cpu().numpy().cls
        classes_inferred = [result.names[i] for i in result_boxes_classes]

        if boxes is None:
            return
        list_yolo_boxes = []
        for box in boxes:
            list_yolo_boxes.append(box)

        vis_frame = result.plot()
        
        for box in list_yolo_boxes:
            x, y, w, h = box
            x1, y1, x2, y2 = int(x - w / 2), int(y - h / 2), int(x + w / 2), int(y + h / 2)
            # add a circle to the center of the bounding box
            cv2.circle(vis_frame, (int(x), int(y)), 5, (0, 255, 0), -1)
            print("X and Y: ", x, y)   
            cv2.rectangle(vis_frame, (x1, y1), (x2, y2), (0, 255, 0), 2)

        # Convert the combined mask to image message
        image_message = self.bridge.cv2_to_imgmsg(vis_frame, encoding="bgr8")
        self.publisher.publish(image_message)

        return classes_inferred, result_boxes_classes, boxes

    def get_segment_results(self, msg: Image):
        """
            Pass in Ros2 Image message and get back yolo segment results 
        """
        pass

        cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        results = self.model(cv_image)
        result = results[0]
        if result is None:
            return None
        
        if result.masks is None:
            return None
        segments = result.masks.cpu().xy
        
        if segments is None:
            return None
        
        list_yolo_segments = []
        for segment in segments:
            yolo_segment = YoloSegment()
            yolo_segment.segment = segment.flatten().tolist()
            list_yolo_segments.append(yolo_segment)

        # initialize the YoloInferences message
        yolo_inference = YoloInferences()

        # Extract the classes of detected objects from the YOLO result
        result_boxes_classes = result.boxes.cpu().numpy().cls

        # Map the class indices to class names
        classes_inferred = [result.names[i] for i in result_boxes_classes]

        # Populate the YoloInferences message with detected classes and segments
        yolo_inference.classes = classes_inferred
        yolo_inference.segments = list_yolo_segments

        # Publish the YoloInferences message
        self.classes_publisher.publish(yolo_inference)
        vis_frame = result.plot()

        # Convert the combined mask to image message
        image_message = self.bridge.cv2_to_imgmsg(vis_frame, encoding="bgr8")
        self.publisher.publish(image_message)

        return classes_inferred, result_boxes_classes, segments

def main():
    rclpy.init()
    yolo_segment_node = YoloSegmentNode()
    rclpy.spin(yolo_segment_node)
    yolo_segment_node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
    