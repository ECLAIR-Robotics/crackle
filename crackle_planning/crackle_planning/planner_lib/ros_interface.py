import rclpy
from rclpy.node import Node

class RosInterface:
    def __init__(self, node : Node, node_name):
        super().__init__(node_name)

    