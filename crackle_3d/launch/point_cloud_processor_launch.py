from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='crackle_3d',
            namespace='crackle_3d',
            executable='point_cloud_preprocessor_node',
            name='sim'
        )
    ])