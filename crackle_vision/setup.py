from setuptools import find_packages, setup
import os
from glob import glob

package_name = "crackle_vision"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]), (os.path.join('share', package_name, 'launch'), glob(os.path.join('launch', '*launch.[pxy][yma]*')))
    ],
    install_requires=[
        "setuptools",
        "ultralytics",
        "std_msgs",
        "sensor_msgs",
        "rclpy",
        "cv_bridge",
        "torch",
        "numpy",
        "crackle_interfaces",
        "xarm_msgs"
    ],
    zip_safe=True,
    maintainer="tanay",
    maintainer_email="tanay.garg@utexas.edu",
    description="TODO: Package description",
    license="Apache-2.0",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "yolo_segment_node = crackle_vision.yolo_segment_node:main",
            "camera_publisher_node = crackle_vision.camera_publisher:main",
            "vision_server_node = crackle_vision.vision_server:main",
        ],
    },
)
