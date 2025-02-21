import rclpy
import rclpy.logging
from rclpy.node import Node
from xarm_msgs.srv import PlanExec, PlanPose, PlanJoint, PlanSingleStraight
from geometry_msgs.msg import Pose, PoseStamped
import threading

class PlannerNode(Node):
    def __init__(self):
        super().__init__('planner_node')
        self.get_logger().info('Planner node started')
        self.xarm_exec_plan = self.create_client(PlanExec, '/xarm_exec_plan')
        self.xarm_plan_pose = self.create_client(PlanPose, '/xarm_pose_plan')
        self.xarm_plan_joint = self.create_client(PlanJoint, '/xarm_joint_plan')
        self.xarm_plan_straight = self.create_client(PlanSingleStraight, '/xarm_straight_plan')
        self.wait_for_service(self.xarm_exec_plan)
        self.wait_for_service(self.xarm_plan_pose)
        self.wait_for_service(self.xarm_plan_joint)
        self.wait_for_service(self.xarm_plan_straight)

    def wait_for_service(self, client):
        while not client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...') 

    def plan_to_pose(self, pose : Pose):
        req = PlanPose.Request()
        req.target = pose
        future = self.xarm_plan_pose.call_async(req)
        print('Sent request')
        rclpy.spin_until_future_complete(self, future)
        print("future: ", future.result())
        return future.result()
        # return resp
    
    def plan_to_joint(self, joint : list):
        req = PlanJoint.Request()
        req.target = joint
        future = self.xarm_plan_joint.call_async(req)
        rclpy.spin_until_future_complete(self, future)
        return future.result()
    
    def plan_straight(self, pose : Pose):
        req = PlanSingleStraight.Request()
        req.target = pose
        future = self.xarm_plan_straight.call_async(req)
        rclpy.spin_until_future_complete(self, future)
        return future.result()

    def execute_done_callback(self, args):
        print('Execution done')

    def execute_plan(self):
        req = PlanExec.Request()
        future = self.xarm_exec_plan.call_async(req)
        future.add_done_callback(self.execute_done_callback)
        # rclpy.spin_until_future_complete(self, future)
        while future.done() == False:
            print(f"{future.done()}")
            rclpy.spin_once(self)
        print('Execution done')
        return future

def accept_input(node):
    while True:
        cmd = input('Enter command: ')
        cmd = cmd.lower()
        cmd = cmd.split()
        if cmd[0] == 'pose':
            pose = Pose()
            pose.position.x = float(cmd[1])
            pose.position.y = float(cmd[2])
            pose.position.z = float(cmd[3])
            pose.orientation.x = float(1.0)
            pose.orientation.y = float(0.0)
            pose.orientation.z = float(0.0)
            pose.orientation.w = float(0.0)
            print('Planned to pose', pose)
            print(node.plan_to_pose(pose))
            print("planned successfully")   

        elif cmd[0] == 'joint':
            joint = [0, 0, 0, 0, 0, 0]
            node.plan_to_joint(joint)

        elif cmd[0] == 'straight':
            pose = Pose()
            pose.position.x = float(cmd[1])
            pose.position.y = float(cmd[2])
            pose.position.z = float(cmd[3])
            pose.orientation.x = float(1.0)
            pose.orientation.y = float(0.0)
            pose.orientation.z = float(0.0)
            pose.orientation.w = float(0.0)
            node.plan_straight(pose)
        elif cmd[0] == 'execute':
            node.execute_plan()
        else:
            exit()

def main(args=None):
    rclpy.init()
    node = PlannerNode()
    print('Created node')
    pose = Pose()
    pose.position.x = float(0.3)
    pose.position.y = float(-0.1)
    pose.position.z = float(0.2)
    pose.orientation.x = float(1.0)
    pose.orientation.y = float(0.0)
    pose.orientation.z = float(0.0)
    pose.orientation.w = float(0.0) 
    # print('Created pose')
    # node.plan_to_pose(pose)
    # print('Planned to pose')
    # node.execute_plan()
    # print('Executed plan')
    accept_input(node)


if __name__ == '__main__':
    main()

# import rclpy
# from rclpy.node import Node


# class MinimalClientAsync(Node):

#     def __init__(self):
#         super().__init__('minimal_client_async')
#         self.cli = self.create_client(PlanPose, '/xarm_pose_plan')
#         self.exec = self.create_client(PlanExec, '/xarm_exec_plan')
#         while not self.cli.wait_for_service(timeout_sec=1.0):
#             self.get_logger().info('service not available, waiting again...')
#         self.req = PlanPose.Request()
#         self.exec_req = PlanExec.Request()

#     def send_request(self, pose: Pose):
#         self.req.target = pose
#         return self.cli.call_async(self.req)
    
#     def execute_request(self):
#         self.exec_req.wait = True
#         return self.exec.call_async(self.exec_req)


# def main():
#     rclpy.init()

#     minimal_client = MinimalClientAsync()
#     pose = Pose()
#     pose.position.x = float(0.3)
#     pose.position.y = float(-0.1)
#     pose.position.z = float(0.2)
#     pose.orientation.x = float(1.0)
#     pose.orientation.y = float(0.0)
#     pose.orientation.z = float(0.0)
#     pose.orientation.w = float(0.0)
#     future = minimal_client.send_request(pose)
#     rclpy.spin_until_future_complete(minimal_client, future)
#     response = future.result()
#     print("response: ", response)

#     minimal_client.execute_request()
#     # minimal_client.get_logger().info(
#     #     'Result of add_two_ints: for %d + %d = %d' %
#     #     (int(sys.argv[1]), int(sys.argv[2]), response.sum))

#     minimal_client.destroy_node()
#     rclpy.shutdown()


# if __name__ == '__main__':
#     main()
