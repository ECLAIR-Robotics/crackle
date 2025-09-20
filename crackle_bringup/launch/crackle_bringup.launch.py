import yaml
import json
from ament_index_python import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
import os
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch.actions import IncludeLaunchDescription, GroupAction, LogInfo, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import SetRemap

def generate_launch_description():

    use_simulated_robot = LaunchConfiguration('simulated', default='false')
    simulate_vision = LaunchConfiguration('simulate_vision', default='false')
    user_simulated_robot_launch_arg = DeclareLaunchArgument(
        'simulated',
        default_value='false',
        description='Use simulated robot if true, else use real robot'
    )
    user_simulate_vision_launch_arg = DeclareLaunchArgument(
        'simulate_vision',
        default_value='true',
        description='Use simulated vision if true, else use real vision'
    )
    log_choice = LogInfo(msg=['Use Simulated Robot: ', use_simulated_robot])
    log_vision_choice = LogInfo(msg=['Use Simulated Vision: ', simulate_vision])

    moveit_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("crackle_moveit"), "launch", "moveit_bringup.launch.py")
        ),
        launch_arguments={'simulated': use_simulated_robot}.items(),
    )
    vision_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("crackle_vision"), "launch", "crackle_vision_launch.py")
        ),
        condition=UnlessCondition(simulate_vision)
    )

    group = GroupAction([
        SetRemap(src='/camera/camera/depth/color/points', dst='/cloud_in'),
    ]) # Revisit this for the octomap



    return LaunchDescription([
        user_simulated_robot_launch_arg,
        user_simulate_vision_launch_arg,
        log_choice,
        log_vision_choice,
        moveit_bringup,
        vision_bringup,
        # group
    ])