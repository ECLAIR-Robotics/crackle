import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node

from arm_interface.action import UfArm

class FibonacciActionServer(Node):

    def __init__(self):
        super().__init__('uf_arm_action_server')
        self._action_server = ActionServer(
            self,
            UfArm,
            'uf_arm',
            self.execute_callback)

    def execute_callback(self, goal_handle):
        print(goal_handle.request.pose)
        self.get_logger().info('Executing goal...')
        result = UfArm.Result()
        result.message = "okay womp womp"
        return result


def main(args=None):
    rclpy.init(args=args)

    fibonacci_action_server = FibonacciActionServer()

    rclpy.spin(fibonacci_action_server)


if __name__ == '__main__':
    main()