from setuptools import find_packages, setup

package_name = 'crackle_planning'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=[
        'setuptools', 
        'rclpy',
        'xarm_msgs', 
        "crackle_interfaces", 
        "std_msgs", 
        "sensor_msgs", 
        "geometry_msgs",
        "moveit_msgs",
        "moveit_commander", 
        "ruckig"
    ],
    zip_safe=True,
    maintainer='tanay',
    maintainer_email='tanay.garg@utexas.edu',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'planner = crackle_planning.planner:main',
            'arm_planner = crackle_planning.arm_planner:main',
        ],
    },
)
