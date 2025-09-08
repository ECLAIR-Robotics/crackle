from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, RegisterEventHandler, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

import os

namespace = "crackle_vision"

def generate_launch_description():

    realsense_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("realsense2_camera"), "launch", "rs_launch.py")
        ),

        launch_arguments= {
            'depth_module.depth_profile': '640x480x30',
            'pointcloud.enable': 'true',
            'align_depth.enable': 'true',
            'enable_depth': 'true',
            'enable_sync': 'true',
            'enable_rgbd': 'true',
            'pointcloud.ordered_pc': 'true',
            'colorizer': 'true'
        }.items()  # Add any specific launch arguments if needed
    )

    vision_server_node = TimerAction(
        period=2.0,  # Wait for 5 seconds before starting the vision server
        actions=[
            Node(
                package="crackle_vision",
                namespace=namespace,
                executable="vision_server_node",
                name="vision_server",
                output="screen",
            )
        ]
    )

    return LaunchDescription([
        # include the realsense camera launch 
        # realsense_launch,
        vision_server_node
    ])