#!/usr/bin/env python3
"""Live read-out of the gripper's grip point in the arm base frame.

Calibration helper for aligning the camera extrinsic (see the hand-eye calibration
plan). The grip point is where the jaws actually close: it sits `kGripPointOffset`
metres along the tool-forward (+Z) axis of the end-effector link. This node looks
up TF (`--ref-frame` -> `--eef-frame`) and prints that point's coordinates in the
reference frame, continuously.

How to use it for the "touch test":
  1. Run this node.
  2. Jog the arm until the gripper is physically, correctly centered on the REAL
     object (as if grasping it).
  3. The printed grip point is then the object's TRUE position in the base frame
     (from the arm's own kinematics — independent of the camera).
  4. Compare that to where perception placed the object (RViz marker / the
     collision object). The difference is the camera-extrinsic error to null out.

Defaults match the crackle lite6 setup: EEF link `link_eef`, base `link_base`,
and kGripPointOffset = 0.175 (keep in sync with
crackle_moveit/include/crackle_moveit/moveit_manipulation.hpp).

Run:
  python3 grip_point_echo.py
  python3 grip_point_echo.py --offset 0.175 --ref-frame link_base --eef-frame link_eef
"""

import argparse

import rclpy
from rclpy.node import Node
from tf2_ros import Buffer, TransformListener, LookupException, ExtrapolationException, ConnectivityException


# Distance (m) from the EEF link along tool +Z to the point between the jaws.
# Keep in sync with kGripPointOffset in moveit_manipulation.hpp.
DEFAULT_OFFSET = 0.175
DEFAULT_REF_FRAME = "link_base"
DEFAULT_EEF_FRAME = "link_eef"


def _quat_rotate(q, v):
    """Rotate vector v (x,y,z) by quaternion q (x,y,z,w). Pure-python, no deps."""
    qx, qy, qz, qw = q
    vx, vy, vz = v
    # t = 2 * cross(q_xyz, v)
    tx = 2.0 * (qy * vz - qz * vy)
    ty = 2.0 * (qz * vx - qx * vz)
    tz = 2.0 * (qx * vy - qy * vx)
    # v' = v + qw*t + cross(q_xyz, t)
    rx = vx + qw * tx + (qy * tz - qz * ty)
    ry = vy + qw * ty + (qz * tx - qx * tz)
    rz = vz + qw * tz + (qx * ty - qy * tx)
    return rx, ry, rz


class GripPointEcho(Node):
    def __init__(self, offset, ref_frame, eef_frame, rate_hz):
        super().__init__("grip_point_echo")
        self.offset = offset
        self.ref_frame = ref_frame
        self.eef_frame = eef_frame
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)
        self.timer = self.create_timer(1.0 / max(rate_hz, 0.1), self._tick)
        self.get_logger().info(
            f"grip point = {offset:.3f} m along +Z of '{eef_frame}', reported in "
            f"'{ref_frame}'. Jog the gripper onto the real object; the printed point "
            f"is the object's true position."
        )

    def _tick(self):
        try:
            tf = self.tf_buffer.lookup_transform(
                self.ref_frame, self.eef_frame, rclpy.time.Time()
            )
        except (LookupException, ExtrapolationException, ConnectivityException) as e:
            self.get_logger().warn(f"TF {self.ref_frame}<-{self.eef_frame} unavailable: {e}", throttle_duration_sec=2.0)
            return

        t = tf.transform.translation
        q = (tf.transform.rotation.x, tf.transform.rotation.y,
             tf.transform.rotation.z, tf.transform.rotation.w)
        # Grip point in EEF frame is (0,0,offset); rotate into ref frame and translate.
        ox, oy, oz = _quat_rotate(q, (0.0, 0.0, self.offset))
        gx, gy, gz = t.x + ox, t.y + oy, t.z + oz
        self.get_logger().info(
            f"eef=({t.x:+.4f}, {t.y:+.4f}, {t.z:+.4f})  "
            f"grip=({gx:+.4f}, {gy:+.4f}, {gz:+.4f})  [{self.ref_frame}]"
        )


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--offset", type=float, default=DEFAULT_OFFSET,
                    help="grip-point offset along EEF +Z, metres (default kGripPointOffset)")
    ap.add_argument("--ref-frame", default=DEFAULT_REF_FRAME, help="frame to report in")
    ap.add_argument("--eef-frame", default=DEFAULT_EEF_FRAME, help="end-effector link")
    ap.add_argument("--rate", type=float, default=2.0, help="print rate (Hz)")
    args, _ = ap.parse_known_args()

    rclpy.init()
    node = GripPointEcho(args.offset, args.ref_frame, args.eef_frame, args.rate)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == "__main__":
    main()
