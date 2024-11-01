from planner_lib._keys import openai_key
from planner_lib.ros_interface import RosInterface
import rclpy
from rclpy.node import Node

class PlannerNode(Node):
    def __init__(self):
        super().__init__('planner_node')

        # Initialize the ROS interface
        self.ros_interface = RosInterface(self, 'ros_interface')

        # Timer to run the planner at a fixed rate
        self.timer = self.create_timer(1.0 / 10.0, self.timer_callback)

def main():
    print('Hi from crackle_planning.')
    print(openai_key)


if __name__ == '__main__':
    main()
