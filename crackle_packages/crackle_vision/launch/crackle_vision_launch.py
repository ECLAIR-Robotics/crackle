from launch import LaunchDescription
from launch_ros.actions import Node

namespace = "crackle_vision"

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="crackle_vision",
            namespace=namespace,
            executable="camera_publisher",
            name="camera_publisher",
            output="screen",
            emulate_tty=True,
        ),
        Node(
            package="crackle_vision",
            namespace=namespace,
            executable="yolo_segment_node",
            name="yolo_segment_node",
            output="screen",
            emulate_tty=True,
        )
    ])