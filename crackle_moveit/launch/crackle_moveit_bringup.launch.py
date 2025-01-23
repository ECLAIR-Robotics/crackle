import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    # 1) Include xArm’s MoveIt launch file
    xarm_moveit = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('xarm_moveit_config'),
                'launch',
                'lite6_moveit_fake.launch.py'
            )
        )
    )

    # 2) Launch your custom planning node
    planner_node = Node(
        package='crackle_moveit',
        executable='task_constructor',
        output='screen',
    )

    return LaunchDescription([
        xarm_moveit,
        planner_node
    ])
