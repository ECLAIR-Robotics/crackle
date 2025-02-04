from geometry_msgs.msg import PoseStamped
import math

MAX_Y_LIMIT = 150
MIN_Y_LIMIT = -150
MAX_X_LIMIT = 150
MIN_X_LIMIT = -150  
MAX_Z_LIMIT = 500
MIN_Z_LIMIT = 500
CRACKLE_GRIPPER_OFFSET = 47

def quat_to_rpy(q):
    sinr_cosp = 2 * (q.w * q.x + q.y * q.z)
    cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y)
    roll = math.atan2(sinr_cosp, cosr_cosp)

    sinp = math.sqrt(1 + 2 * (q.w * q.y - q.x * q.z))
    cosp = math.sqrt(1 - 2 * (q.w * q.y - q.x * q.z))
    pitch = 2 * atan2(sinp, cosp) - math.pi / 2

    siny_cosp = 2 * (q.w * q.z + q.x * q.y)
    cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z)
    yaw = atan2(siny_cosp, cosy_cosp)

    return (roll, pitch, yaw)

class MoveArmNode(Node):

    def __init__(self):
        super().__init__("move_arm_node")
        self.target_pose_sub = self.create_subscription(PoseStamped, "target_pose", self.move_to_target_pose, 10)
    

    def move_to_target_pose(self, msg: PoseStamped):        
        request = MoveCartesian.Request()

        position = msg.pose.position

        request.pose = [
            max(MIN_X_LIMIT, min(MAX_X_LIMIT, position.x)), 
            max(MIN_Y_LIMIT, min(MAX_Y_LIMIT, position.y)),
            max(MIN_Z_LIMIT, min(MAX_Z_LIMIT, position.z)),
            *quat_to_rpy(msg.pose.orientation)
        ] # [x, y, z, roll, pitch, yaw]

        request.speed = 15.0
        request.acc = 8.0 
        request.mvtime = 7.0
        request.wait = True
        request.timeout = 15.0
        print(request)

        future = self.arm_set_position_client.call_async(request)
        
        rclpy.spin_until_future_complete(self, future)

def main():
    rclpy.init()
    move_arm_node = MoveArmNode()
    rclpy.spin(move_arm_node)
    move_arm_node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()