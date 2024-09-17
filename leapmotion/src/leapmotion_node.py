import leap, _thread, sys, time
# from leap import CirleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture
import rclpy

class LeapMotionNode(leap.Listener, Node):
    finger_names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
    bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']
    state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']
    def __init__(self):
        self.controller = leap.Controller()
        self.controller.add_listener(self)
        self.frame = None
        self.gestures = []
        self.hands = []
        self.fingers = []
        self.bones = []

    def on_init(self, controller):
        print("Initialized")

    def on_connect(self, controller):
        print ("Connected")
        controller.enable_gesture()
        controller.enable_gesture(KeyTapGesture.GESTURE_TYPE)
        controller.enable_gesture(ScreenTapGesture.GESTURE_TYPE)
        controller.enable_gesture(SwipeGesture.GESTURE_TYPE)

    def on_disconnect(self, controller):
        print ("Disconnected")

    def on_exit(self, controller):
        print ("Exited")

    def on_frame(self, controller):
        self.frame = controller.frame()
        self.gestures = self.frame.gestures()
        self.hands = self.frame.hands()
        self.fingers = self.frame.fingers()
        self.bones = self.frame.bones()

    def get_frame(self):
        return self.frame

    def get_gestures(self):
        return self.gestures

    def get_hands(self):
        return self.hands

    def get_fingers(self):
        return self.fingers

    def get_bones(self):
        return self.bones

    def get_hand(self, index):
        return self.hands[index]

    def get_finger(self, index):
        return self.fingers[index]

    def get_bone(self, index):
        return self.bones[index]

    def get_joint_position(self, joint):
        return joint.position

    def get_joint_direction(self, joint):
        return joint.direction

    def get_joint_normal(self, joint):
        return joint.normal

    def get_joint_velocity(self, joint):
        return joint.velocity

    def get_joint_width(self, joint):
        return joint.width

    def get_joint_length(self, joint):
        return joint.length

    def get_joint_is_valid(self, joint):
        return joint.is_valid

    def get_joint_is_left(self, joint):
        return joint.is_left

    def get_joint_is_right(self, joint):
        return joint.is_right

    def get_joint_is_thumb(self, joint):
        return joint.is_thumb

    def get_joint_is_index(self, joint):
        return joint.is_index

    def get_joint_is_middle(self, joint):
        return joint.is_middle

    def get_joint_is_ring(self, joint):
        return joint.is_ring

    def get_joint_is_pinky(self, joint):
        return joint.is_pinky

def main():
    listener = LeapMotionNode()
    controller = leap.Controller()

    controller.add_listener(listener)
    print("Press Enter to quit...")
    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        controller.remove_listener(listener)

if __name__ == "__main__":
    main()