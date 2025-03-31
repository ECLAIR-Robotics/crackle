from ament_index_python import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
import os

def generate_launch_description():
    pkg_name = 'crackle_description'
    urdf_file = os.path.join(
        get_package_share_directory(pkg_name),
        'urdf',
        'crackle_gripper.urdf.xacro'
    )

    doc = xacro.process_file(urdf_file)
    robot_description_config = doc.toxml()

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description_config}]
    )

    joint_state_publisher_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui',
        output='screen',
        parameters=[{'use_gui': 'true'}]
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', os.path.join(get_package_share_directory(pkg_name), 'rviz', 'description_rviz.rviz')]
    )
    
    return LaunchDescription([
       joint_state_publisher_node,
       robot_state_publisher_node,
       rviz_node 
    ])