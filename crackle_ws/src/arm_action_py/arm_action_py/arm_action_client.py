import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node

from arm_interface.action import UfArm


class FibonacciActionClient(Node):

    def __init__(self):
        super().__init__('uf_arm_action_client')
        self._action_client = ActionClient(self, UfArm, 'uf_arm')

    def send_goal(self, order):
        
        goal_msg = UfArm.Goal()
        goal_msg.pose = "go jump off the cliff"

        self._action_client.wait_for_server()

        return self._action_client.send_goal_async(goal_msg)


def main(args=None):
    rclpy.init(args=args)

    action_client = FibonacciActionClient()

    future = action_client.send_goal(10)

    rclpy.spin_until_future_complete(action_client, future)


if __name__ == '__main__':
    main()