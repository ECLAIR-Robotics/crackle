import time
import leap
from leap import datatypes as ldt


class PointingListener(leap.Listener):
    def on_tracking_event(self, event):
        for hand in event.hands:
            vector = vector_direction(hand, 1)
            print(f"X: {vector.x}, Y: {vector.y}, Z: {vector.z}")
            
            # print(vector)



def vector_direction(hand: ldt.Hand, digit_idx: int) -> ldt.Vector:
    digit = hand.digits[digit_idx]
    return digit.distal.next_joint

def main():
    print("Connecting...")
    listener = PointingListener()

    connection = leap.Connection()
    connection.add_listener(listener)

    with connection.open():
        while True:
            time.sleep(1)


if __name__ == "__main__":
    main()