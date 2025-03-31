from setuptools import find_packages, setup
import os
from glob import glob

package_name = "crackle_3d"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
        (os.path.join('share', package_name, 'launch'), 
        glob(os.path.join('launch', '*launch.[pxy][yma]*')))
    ],
    install_requires=[
        "setuptools",
        "numpy",
        "open3d",
        "rclpy",
        "sensor_msgs",
        "std_msgs",
        "crackle_interfaces",
    ],
    zip_safe=True,
    maintainer="tanay",
    maintainer_email="tanay.garg@utexas.edu",
    description="TODO: Package description",
    license="TODO: License declaration",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "point_cloud_preprocessor_node = crackle_3d.point_cloud_preprocessor:main",
        ],
    },
)
