import rclpy
from rclpy.node import Node
from std_msgs.msg import String

from std_srvs.srv import Empty

import rclpy

def service_cb(request, response):
    print("service_cb()")
    return response

def main():
    rclpy.init()
    node = rclpy.create_node('test_service')
    srv = node.create_service(Empty, 'my_service', service_cb)
    rclpy.spin(node)

if __name__ == '__main__':
    main()