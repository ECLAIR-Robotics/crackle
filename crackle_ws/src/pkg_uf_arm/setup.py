from setuptools import find_packages, setup

package_name = 'pkg_uf_arm'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='tanay',
    maintainer_email='tanay@todo.todo',
    description='TODO: Package description',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'uf_arm = pkg_uf_arm.uf_arm:main',
            'test_node=pkg_uf_arm.test_node:main'
        ],
    },
)
