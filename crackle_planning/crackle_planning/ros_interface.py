import rclpy
from crackle_interfaces.srv import PickupObject
from rclpy.node import Node

class RosInterface:
    def __init__(self, node : Node):
        self._node = node
        self._pickup_service_client = node.create_client(PickupObject, 'crackle_manipulation/pickup_object')
 
    def call_pickup_service(self, object_name: str):
        while not self._pickup_service_client.wait_for_service(timeout_sec=1.0):
            self._node.get_logger().info('Service not available, waiting again...')
        request = PickupObject.Request()
        request.object_name = object_name
        future = self._pickup_service_client.call_async(request)
        rclpy.spin_until_future_complete(self._node, future)
        return future.result()
