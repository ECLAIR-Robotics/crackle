#!/usr/bin/env python3
# Software License Agreement (BSD License)
#
# Copyright (c) 2021, UFACTORY, Inc.
# All rights reserved.
#
# Author: Vinman <vinman.wen@ufactory.cc> <vinman.cub@gmail.com>

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    hw_ns = LaunchConfiguration('hw_ns', default='ufactory')

    # robot moveit fake launch
    # xarm_moveit_config/launch/_robot_moveit_fake.launch.py
    robot_moveit_fake_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution([FindPackageShare('xarm_moveit_config'), 'launch', '_robot_moveit_fake.launch.py'])),
        launch_arguments={
            'dof': '6',
            'robot_type': 'uf850',
            'hw_ns': hw_ns,
            'no_gui_ctrl': 'true',
        }.items(),
    )
    
    return LaunchDescription([
        robot_moveit_fake_launch
    ])
