import rclpy
from rclpy.node import Node
from rclpy.node import ActionServer
from std_msgs.msg import Float64MultiArray

import robotpy_apriltag
import cv2
import numpy as np
from typing import List, Tuple
import glob
import detect_apriltag

class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, '/topic', 10) # keep Float64 for now, later it will be changed though
        timer_period = 0.5  # in seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0


    def timer_callback(self):
        msg = Float64MultiArray() # TODO: Look at the schema, what properties it has
        msg.data = detect_apriltag.april_tag_detector.WIDTH #"WIDTH" just to test # M is supposed to go here
        # msg.data = 'Hello World: %d' % self.i
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)
        # self.i += 1
        # This will eventually publish the image

def main():
    print('Hi from pkg_photo.')

    rclpy.init(args=args)

    april_tag_detector = detect_apriltag.AprilTags()
    print('Test (Width): ', april_tag_detector.WIDTH)
    
    april_tag_detector.getHomography()

    minimal_publisher = MinimalPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()