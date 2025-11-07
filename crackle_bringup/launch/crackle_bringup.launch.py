import yaml
import json
from ament_index_python import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
import os
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch.actions import IncludeLaunchDescription, GroupAction, LogInfo, DeclareLaunchArgument, TimerAction
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

    moveit_bringup = TimerAction(
        period=5.0,  # Wait for 2 seconds before starting the moveit bringup
        actions=[
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    os.path.join(get_package_share_directory("crackle_moveit"), "launch", "moveit_bringup.launch.py")
                ),
                launch_arguments={'simulated': use_simulated_robot}.items(),
            )
        ]
    )
    vision_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("crackle_vision"), "launch", "crackle_vision_launch.py")
        ),
        condition=UnlessCondition(simulate_vision)
    )

    face_node = Node(
        package='crackle_face_node',
        executable='face_node',
        name='face_node',
        output='screen',
        parameters=[]
    )

    audio_localization_node = Node(
        package='sound_direction_publisher',
        executable='sound_direction_publisher',
        name='sound_direction_publisher',
        output='screen',
        parameters=[{'use_sim_time': use_simulated_robot}]
    )

    group = GroupAction([
        SetRemap(src='/camera/camera/depth/color/points', dst='/cloud_in'),
    ]) # Revisit this for the octomap


    return LaunchDescription([
        user_simulated_robot_launch_arg,
        user_simulate_vision_launch_arg,
        log_choice,
        log_vision_choice,
        vision_bringup,
        moveit_bringup,
        face_node,
        audio_localization_node,
        # group
    ])