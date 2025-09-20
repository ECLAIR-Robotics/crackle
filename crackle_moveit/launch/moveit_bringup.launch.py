from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch.actions import IncludeLaunchDescription, TimerAction, DeclareLaunchArgument, LogInfo
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

import os




def generate_launch_description():
    # define a launch argument for if simulated robot is used
    use_simulated_robot = LaunchConfiguration('simulated', default='false')
    user_simulated_robot_launch_arg = DeclareLaunchArgument(
        'simulated',
        default_value='false',
        description='Use simulated robot if true, else use real robot'
    )
    log_choice = LogInfo(msg=['Use Simulated Robot: ', use_simulated_robot])
    # print the value of the launch argument

    sim_moveit_bringup_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("xarm_moveit_config"), "launch", "lite6_moveit_fake.launch.py")
        ),
        condition = IfCondition(use_simulated_robot)
    )
    real_moveit_bringup_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("xarm_moveit_config"), "launch", "lite6_moveit_realmove.launch.py")
        ),
        launch_arguments={'robot_ip': '192.168.1.166'}.items(),
        condition=UnlessCondition(use_simulated_robot)
        
    )


    # include scene_update.launch.py
    scene_updater_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("crackle_moveit"), "launch", "scene_updater.launch.py")
        ),
    )

    timer_action_scene_updater = TimerAction(
        period=5.0,  # Wait for 5 seconds before starting the scene updater
        actions=[scene_updater_launch]
    )
    return LaunchDescription([
        user_simulated_robot_launch_arg,
        log_choice,
        sim_moveit_bringup_launch,
        real_moveit_bringup_launch,
        timer_action_scene_updater
    ])
