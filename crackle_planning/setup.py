from setuptools import find_packages, setup 

package_name = 'crackle_planning'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test'], include=[package_name, package_name + '.*']),
    # Ship the whimsical face UI's HTML alongside the Python so the server can
    # find it whether run from source or an installed (colcon) space.
    package_data={package_name + '.face_ui': ['index.html']},
    include_package_data=True,
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/face_ui',
            ['crackle_planning/face_ui/index.html']),
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
            'main_node = crackle_planning.main:main',
            'api_test_node = crackle_planning.api_test_node:main',
            'face_ui = crackle_planning.face_ui.server:main',
        ],
    },
)
