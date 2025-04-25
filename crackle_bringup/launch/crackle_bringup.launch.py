import yaml
import json
from ament_index_python import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
import os
from launch.actions import IncludeLaunchDescription, GroupAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import SetRemap
from uf_ros_lib.moveit_configs_builder import MoveItConfigsBuilder

def generate_launch_description():
    
    # Realsense D435i file
    realsense_launch_file = os.path.join(
        get_package_share_directory('realsense2_camera'),
        'launch',
        'rs_launch.py'
    )

    realsense_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(realsense_launch_file),
        launch_arguments={
            'depth_module.depth_profile': '1280x720x30',
            'pointcloud.enable': 'true',
            }.items()
    )

    xarm_moveit_config_dir = get_package_share_directory('xarm_moveit_config')
    xarm_moveit_config_file = os.path.join(
        xarm_moveit_config_dir,
        'launch',
        'lite6_moveit_realmove.launch.py'
    )

    xarm_realmove_include = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(xarm_moveit_config_file),
        launch_arguments={
            'robot_ip': '192.168.1.166',
        }.items()
    )

    group = GroupAction([
        SetRemap(src='/camera/camera/depth/color/points', dst='/cloud_in'),
        realsense_launch
    ])



    return LaunchDescription([
        group
    ])