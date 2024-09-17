import leap
import numpy as np
import cv2
import rclpy
from xarm_msgs.srv import MoveCartesian
from rclpy.node import Node
import time


_TRACKING_MODES = {
    leap.TrackingMode.Desktop: "Desktop",
    leap.TrackingMode.HMD: "HMD",
    leap.TrackingMode.ScreenTop: "ScreenTop",
}

class AsyncServiceClient(Node):
    def __init__(self):
        super().__init__('async_service_client')
        self.cli = self.create_client(MoveCartesian, '/ufactory/set_position')
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('service not available, waiting again...')
        self.req = MoveCartesian.Request()
        self.lasttime = time.time()
    def send_request(self, x, y, z, roll, pitch, yaw):
        # x: 100 - 400
        # y: -150 - 150
        # z: 100 - 250
        if (time.time() - self.lasttime < 2.0):
            return
        print("Moving arm to: ", x, y, z)
        y += 200
        z -= 100
        self.lasttime = time.time()
        if (y < 90 or y > 400):
            y = 100.0
        if (x < -150 or x > 150):
            x = 0.0
        if (z < 150 or z > 300):
            z = 200.0
        print("[Updated] Moving arm to: ", x, y, z)
        self.req.pose = [x, y, z, roll, pitch, yaw]
        self.req.speed = 70.0
        self.req.acc = 100.0
        self.req.mvtime = 1.0
        self.future = self.cli.call_async(self.req)
        rclpy.spin_until_future_complete(self, self.future)
        return self.future.result()


class Canvas:
    def __init__(self, client: AsyncServiceClient):
        self.name = "Python Gemini Visualiser"
        self.screen_size = [500, 700]
        self.hands_colour = (255, 255, 255)
        self.font_colour = (0, 255, 44)
        self.hands_format = "Skeleton"
        self.output_image = np.zeros((self.screen_size[0], self.screen_size[1], 3), np.uint8)
        self.tracking_mode = None
        self.client = client

    def set_tracking_mode(self, tracking_mode):
        self.tracking_mode = tracking_mode

    def toggle_hands_format(self):
        self.hands_format = "Dots" if self.hands_format == "Skeleton" else "Skeleton"
        print(f"Set hands format to {self.hands_format}")

    def get_joint_position(self, bone):
        if bone:
            return int(bone.x + (self.screen_size[1] / 2)), int(bone.z + (self.screen_size[0] / 2))
        else:
            return None

    def render_hands(self, event):
        # Clear the previous image
        self.output_image[:, :] = 0

        cv2.putText(
            self.output_image,
            f"Tracking Mode: {_TRACKING_MODES[self.tracking_mode]}",
            (10, self.screen_size[0] - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.5,
            self.font_colour,
            1,
        )

        if len(event.hands) == 0:
            return

        for i in range(0, len(event.hands)):
            hand = event.hands[i]
            # print("Hand position: ", hand.digits[0].bones[0].prev_joint.x, hand.digits[0].bones[0].prev_joint.y, hand.digits[0].bones[0].prev_joint.z)
            joint = hand.digits[0].bones[0].prev_joint
            req_x = ((joint.x + 242) / 210) + 150
            req_y = ((joint.z + 114) / 250) - 50    
            req_z = ((joint.y - 150) / 300) + 150
            # print("Moving arm to: ", req_x, req_y, req_z, joint.x, joint.y, joint.z)
            # self.client.send_request(req_x, req_y, req_z, 3.14, 0.0, 0.0)
            self.client.send_request(joint.z, joint.x, joint.y, 3.14, 0.0, 0.0)

            for index_digit in range(0, 5):
                digit = hand.digits[index_digit]
                for index_bone in range(0, 4):
                    bone = digit.bones[index_bone]
                    if self.hands_format == "Dots":
                        prev_joint = self.get_joint_position(bone.prev_joint)
                        next_joint = self.get_joint_position(bone.next_joint)
                        if prev_joint:
                            cv2.circle(self.output_image, prev_joint, 2, self.hands_colour, -1)

                        if next_joint:
                            cv2.circle(self.output_image, next_joint, 2, self.hands_colour, -1)

                    if self.hands_format == "Skeleton":
                        wrist = self.get_joint_position(hand.arm.next_joint)
                        elbow = self.get_joint_position(hand.arm.prev_joint)
                        if wrist:
                            cv2.circle(self.output_image, wrist, 3, self.hands_colour, -1)

                        if elbow:
                            cv2.circle(self.output_image, elbow, 3, self.hands_colour, -1)

                        if wrist and elbow:
                            cv2.line(self.output_image, wrist, elbow, self.hands_colour, 2)

                        bone_start = self.get_joint_position(bone.prev_joint)
                        bone_end = self.get_joint_position(bone.next_joint)

                        if bone_start:
                            cv2.circle(self.output_image, bone_start, 3, self.hands_colour, -1)

                        if bone_end:
                            cv2.circle(self.output_image, bone_end, 3, self.hands_colour, -1)

                        if bone_start and bone_end:
                            cv2.line(self.output_image, bone_start, bone_end, self.hands_colour, 2)

                        if ((index_digit == 0) and (index_bone == 0)) or (
                            (index_digit > 0) and (index_digit < 4) and (index_bone < 2)
                        ):
                            index_digit_next = index_digit + 1
                            digit_next = hand.digits[index_digit_next]
                            bone_next = digit_next.bones[index_bone]
                            bone_next_start = self.get_joint_position(bone_next.prev_joint)
                            if bone_start and bone_next_start:
                                cv2.line(
                                    self.output_image,
                                    bone_start,
                                    bone_next_start,
                                    self.hands_colour,
                                    2,
                                )

                        if index_bone == 0 and bone_start and wrist:
                            cv2.line(self.output_image, bone_start, wrist, self.hands_colour, 2)


class TrackingListener(leap.Listener):
    def __init__(self, canvas):
        self.canvas = canvas

    def on_connection_event(self, event):
        pass

    def on_tracking_mode_event(self, event):
        self.canvas.set_tracking_mode(event.current_tracking_mode)
        print(f"Tracking mode changed to {_TRACKING_MODES[event.current_tracking_mode]}")

    def on_device_event(self, event):
        try:
            with event.device.open():
                info = event.device.get_info()
        except leap.LeapCannotOpenDeviceError:
            info = event.device.get_info()
        print(f"Found device {info.serial}")

    def on_tracking_event(self, event):
        self.canvas.render_hands(event)
# class GestureListener(leap.Listener):
#     def on_gesture_event(self, event):
#         if event.gesture_type == leap.GestureType.Swipe:
#             print("Swipe gesture detected")
#         elif event.gesture_type == leap.GestureType.Circle:
#             print("Circle gesture detected")
#         elif event.gesture_type == leap.GestureType.KeyTap:
#             print("KeyTap gesture detected")
#         elif event.gesture_type == leap.GestureType.ScreenTap:
#             print("ScreenTap gesture detected")

def main():
    rclpy.init()
    client = AsyncServiceClient()
    canvas = Canvas(client=client)

    print(canvas.name)
    print("")
    print("Press <key> in visualiser window to:")
    print("  x: Exit")
    print("  h: Select HMD tracking mode")
    print("  s: Select ScreenTop tracking mode")
    print("  d: Select Desktop tracking mode")
    print("  f: Toggle hands format between Skeleton/Dots")

    tracking_listener = TrackingListener(canvas)
    
    connection = leap.Connection()

    connection.add_listener(tracking_listener)
    running = True

    with connection.open():
        connection.set_tracking_mode(leap.TrackingMode.Desktop)
        canvas.set_tracking_mode(leap.TrackingMode.Desktop)
        
        while running:
            cv2.imshow(canvas.name, canvas.output_image)
            
            
            key = cv2.waitKey(1)

            if key == ord("x"):
                rclpy.shutdown()
                break
            elif key == ord("h"):
                connection.set_tracking_mode(leap.TrackingMode.HMD)
            elif key == ord("s"):
                connection.set_tracking_mode(leap.TrackingMode.ScreenTop)
            elif key == ord("d"):
                connection.set_tracking_mode(leap.TrackingMode.Desktop)
            elif key == ord("f"):
                canvas.toggle_hands_format()


if __name__ == "__main__":
    main()
