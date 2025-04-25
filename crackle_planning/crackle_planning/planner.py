from crackle_planning._keys import key
from crackle_planning.ros_interface import RosInterface
from crackle_planning._llm import GptAPI
import rclpy
from rclpy.node import Node

class PlannerNode(Node):
    def __init__(self):
        super().__init__('planner_node')

        # Initialize the ROS interface
        self.ros_interface = RosInterface(self)
        

        # Timer to run the planner at a fixed rate
        self.timer = self.create_timer(1.0 / 10.0, self.timer_callback)

def main():
    print('Hi from crackle_planning.')
    api=GptAPI(key)
    print('Works')
    prompt='Pick up the object'
    response=api.get_command(prompt)
    print(response)
    print(key)


if __name__ == '__main__':
    main()
