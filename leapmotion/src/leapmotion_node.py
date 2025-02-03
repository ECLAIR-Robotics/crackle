import leap, _thread, sys, time
import rclpy
from rclpy.node import Node
from crackle_interfaces.msg import LeapMotionHand

class LeapMotionNode(leap.Listener, Node):
    finger_names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
    bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']
    state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']
    def __init__(self):
        super().__init__('hand_direction_publisher')
        self.publisher_ = self.create_publisher(LeapMotionHand, "hand_direction_topic", 1)
        self.hand = None
        self.most_recent_frame = 0
        self.skip = 1

    def on_connection_event(self, event):
        print("Connected")

    def on_device_event(self, event):
        try:
            with event.device.open():
                info = event.device.get_info()
        except leap.LeapCannotOpenDeviceError:
            info = event.device.get_info()

        print(f"Found device {info.serial}")

    def on_tracking_event(self, event):
        self.most_recent_frame = event.tracking_frame_id
        if event.tracking_frame_id % self.skip == 0:
            print(f"Frame {event.tracking_frame_id} with {len(event.hands)} hands.")
            for hand in event.hands:
                hand_type = "left" if str(hand.type) == "HandType.Left" else "right"
                print(
                    f"Hand id {hand.id} is a {hand_type} hand with position ({hand.palm.position.x}, {hand.palm.position.y}, {hand.palm.position.z})."
                )
            if (len(event.hands) > 0):
                print(event.hands[0])
                hands = event.hands
                # hands = sorted(hands, key=lambda hand: hand.confidence, reverse=True)
                # print("Confidence: ", hands[0].confidence)
                self.hand = max(event.hands, key=lambda hand: hand.confidence)
                print("Self.hand: ", self.hand)
                self.frame_callback()
    
    def frame_callback(self):
        print(self.hand.palm)
        dir = self.hand.palm.direction
        pos = self.hand.palm.stabilized_position
        print("Direction: ", dir)
        to_publish = LeapMotionHand()
        to_publish.hand_dir.x = dir.x()
        to_publish.hand_dir.y = dir.y()
        to_publish.hand_dir.z = dir.z()
        to_publish.hand_pos.x = pos.x()
        to_publish.hand_pos.x = pos.y()
        to_publish.hand_pos.x = pos.z()

        self.publisher_.publish(to_publish)


def main(args = None):
    rclpy.init(args=args)
    listener = LeapMotionNode()
    connection = leap.Connection()

    connection.add_listener(listener)

    running = True

    with connection.open():
        connection.set_tracking_mode(leap.TrackingMode.Desktop)
        while running:
            time.sleep(1)

    print("Press Enter to quit...")
    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        listener.destroy_node()
        rclpy.shutdown()

        connection.remove_listener(listener)
        connection._destroy_connection()

if __name__ == "__main__":
    main()