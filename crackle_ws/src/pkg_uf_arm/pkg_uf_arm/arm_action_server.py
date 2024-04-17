import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node
from share.pkg_uf_arm.action import UfArm


class UfArmActionServer(Node):

    def __init__(self):
        super().__init__('UfArm_action_server')
        self._action_server = ActionServer(
            self,
            UfArm,
            'UfArm',
            self.execute_callback)

    def execute_callback(self, goal_handle):
        self.get_logger().info('Executing goal...')
        result = UfArm.Result()
        return result


def main(args=None):
    rclpy.init(args=args)

    fibonacci_action_server = UfArmActionServer()

    rclpy.spin(fibonacci_action_server)


if __name__ == '__main__':
    main()