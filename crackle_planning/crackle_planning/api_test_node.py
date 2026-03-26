from typing import Optional

import rclpy
from rclpy.node import Node
from std_srvs.srv import Trigger

from crackle_planning._api import PlannerAPI


class ApiTestNode(Node):
    """Small test harness node for PlannerAPI pick/place methods."""

    def __init__(self):
        super().__init__("api_test_node")

        self.declare_parameter("pickup_object_name", "bottle")
        self.declare_parameter("startup_action", "none")  # none|pickup|place|both

        self.api = PlannerAPI(use_ros=True)

        self._pickup_service = self.create_service(
            Trigger,
            "~/pickup",
            self._pickup_callback,
        )
        self._place_service = self.create_service(
            Trigger,
            "~/place",
            self._place_callback,
        )

        self._startup_done = False
        self._startup_timer = self.create_timer(0.75, self._run_startup_action_once)

        self.get_logger().info("API test node ready.")
        self.get_logger().info("Call ~/pickup or ~/place to test PlannerAPI methods.")

    def _pickup_object_name(self) -> str:
        return (
            self.get_parameter("pickup_object_name")
            .get_parameter_value()
            .string_value
        )

    def _pickup_callback(self, _request: Trigger.Request, response: Trigger.Response):
        object_name = self._pickup_object_name()
        try:
            self.get_logger().info(f"Testing PlannerAPI.pick_up('{object_name}')")
            self.api.pick_up(object_name)
            response.success = True
            response.message = f"pick_up() invoked for '{object_name}'"
        except Exception as exc:
            response.success = False
            response.message = f"pick_up() failed: {exc}"
            self.get_logger().error(response.message)
        return response

    def _place_callback(self, _request: Trigger.Request, response: Trigger.Response):
        try:
            self.get_logger().info("Testing PlannerAPI.place()")
            self.api.place()
            response.success = True
            response.message = "place() invoked"
        except Exception as exc:
            response.success = False
            response.message = f"place() failed: {exc}"
            self.get_logger().error(response.message)
        return response

    def _run_startup_action_once(self):
        if self._startup_done:
            return

        self._startup_done = True
        self._startup_timer.cancel()

        action = (
            self.get_parameter("startup_action")
            .get_parameter_value()
            .string_value
            .strip()
            .lower()
        )

        if action == "none":
            return

        if action == "pickup":
            self.api.pick_up(self._pickup_object_name())
        elif action == "place":
            self.api.place()
        elif action == "both":
            self.api.pick_up(self._pickup_object_name())
            self.api.place()
        else:
            self.get_logger().warn(
                "Invalid startup_action. Use one of: none, pickup, place, both."
            )


def main(args: Optional[list] = None):
    rclpy.init(args=args)
    node = ApiTestNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.api._shutdown_ros()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
