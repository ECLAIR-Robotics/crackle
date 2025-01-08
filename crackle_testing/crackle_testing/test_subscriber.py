import rclpy
from rclpy.node import Node
from rclpy.executors import MultiThreadedExecutor
from std_msgs.msg import String
from std_srvs.srv import Empty

class MyNode(Node):
    def __init__(self):
        super().__init__('my_node')

        # Create a subscriber
        self.sub_ = self.create_subscription(
            String,
            'topic',
            self.topic_callback,
            10
        )
        # Create a service client
        self.client_ = self.create_client(Empty, 'my_service')
        print("Node initialized")
        self.future_callbacks = []
        self.service_callback = None

    def func_callback(self, *args):
        print("Callback called")

    def topic_callback(self, msg):
        print(f"Received message: {msg.data}")
        # Synchronous call to service1
        request = Empty.Request()
        # fill in request...
        f = self.client_.call_async(request)
        f.add_done_callback(self.func_callback)
        self.future_callbacks.append(f)
        print()
    
    def spin(self):
        print("Spinning")
        while rclpy.ok():
            rclpy.spin_once(self)
            incomplete_futures = [] 
            print(self.future_callbacks)
            for f in self.future_callbacks:
                if f is not None and f.done():
                    try:
                        res = f.result()
                        print("Service call successfull, result = ", res)
                        
                    except Exception as e:
                        self.get_logger().error(f"Error in callback: {e}")
                else:
                    incomplete_futures.append(f)
            self.future_callbacks = incomplete_futures

def main(args=None):
    rclpy.init(args=args)
    node = MyNode()

    node.spin()

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()