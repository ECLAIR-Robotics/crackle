#!/usr/bin/env python3
"""
Eye-in-hand calibration for the wrist-mounted RealSense on the Lite6 arm.

Recovers the fixed transform  gripper_base -> camera_depth_optical_frame
(i.e. the origin of `camera_depth_optical_frame_joint` in
crackle_gripper.urdf.xacro) using a ChArUco board that is held STILL in the
world while the arm is jogged to several poses that all view the board.

At each captured sample we record:
  * base_frame -> hand_frame           (from TF, i.e. gripper_base in link_base)
  * camera(color) -> board             (from ChArUco solvePnP on the color image)

cv2.calibrateHandEye then solves AX = XB for X = hand_frame -> camera(color).
Finally X is shifted into the DEPTH optical frame (where the point cloud is
published) using the driver's depth_to_color extrinsic, and printed as
xyz + rpy ready to paste into the xacro joint.

Board (auto-detected from /home/tanay/Downloads/board.png):
  10 x 8 squares, DICT_ARUCO_ORIGINAL, LEGACY pattern,
  square = 22.5 mm, marker ~ 10.1 mm.

Usage (arm + RealSense + robot_state_publisher already running):
  python3 handeye_calibrate.py
Then in the terminal:
  <Enter> / c : capture a sample at the current pose (hold the arm still)
  s           : solve and print the calibration so far
  u           : undo last sample
  w           : write the current solution into the xacro joint
  q           : quit

Recommended: 10-15 samples with WELL-VARIED wrist orientation (not just
translation) and the board filling a good part of the frame.
"""

import os
import sys
import select
import threading
import datetime

import numpy as np
import cv2

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge

from tf2_ros import Buffer, TransformListener
import tf2_ros

try:
    from realsense2_camera_msgs.msg import Extrinsics
    HAVE_EXTRINSICS = True
except Exception:
    HAVE_EXTRINSICS = False

# ----------------------------- configuration -------------------------------
BASE_FRAME  = 'link_base'
HAND_FRAME  = 'gripper_base'          # parent of the camera joint in the xacro
COLOR_TOPIC = '/camera/camera/color/image_raw'
INFO_TOPIC  = '/camera/camera/color/camera_info'
EXTR_TOPIC  = '/camera/camera/extrinsics/depth_to_color'

SQUARES_X, SQUARES_Y = 10, 8
SQUARE_LEN = 0.0225                   # metres
MARKER_LEN = 0.0225 * 74.0 / 165.0    # ~0.01009 m, measured from board.png
ARUCO_DICT = cv2.aruco.DICT_ARUCO_ORIGINAL
LEGACY_PATTERN = True
MIN_CORNERS = 8                        # min charuco corners to accept a sample

XACRO = os.path.expanduser(
    '~/crackle_ws/src/crackle/crackle_description/urdf/crackle_gripper.urdf.xacro')
SAVE_DIR = os.path.expanduser('~/crackle_ws/handeye_calib')

# Fallback depth->color extrinsic used ONLY if the driver's
# /extrinsics/depth_to_color topic never arrives (it is a one-shot latched
# topic that sometimes fails to deliver). This is the nominal D435/D435i value:
# the colour sensor sits ~14.85 mm along +X from the depth (left-IR) sensor,
# with rotation ~ identity. Per-unit variation is sub-millimetre / <0.3 deg.
DEFAULT_DEPTH_TO_COLOR_T = np.array([0.01485, 0.0, 0.0])

# ------------------------------- math utils --------------------------------

def quat_to_mat(x, y, z, w):
    n = np.sqrt(x * x + y * y + z * z + w * w)
    x, y, z, w = x / n, y / n, z / n, w / n
    return np.array([
        [1 - 2 * (y * y + z * z), 2 * (x * y - z * w),     2 * (x * z + y * w)],
        [2 * (x * y + z * w),     1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
        [2 * (x * z - y * w),     2 * (y * z + x * w),     1 - 2 * (x * x + y * y)],
    ])


def mat_to_rpy(R):
    """URDF fixed-axis rpy: R = Rz(yaw) Ry(pitch) Rx(roll) -> (roll,pitch,yaw)."""
    sy = -R[2, 0]
    sy = max(-1.0, min(1.0, sy))
    pitch = np.arcsin(sy)
    if abs(sy) < 0.99999:
        roll = np.arctan2(R[2, 1], R[2, 2])
        yaw = np.arctan2(R[1, 0], R[0, 0])
    else:  # gimbal lock
        roll = np.arctan2(-R[1, 2], R[1, 1])
        yaw = 0.0
    return roll, pitch, yaw


def make_T(R, t):
    T = np.eye(4)
    T[:3, :3] = R
    T[:3, 3] = np.asarray(t).reshape(3)
    return T


def inv_T(T):
    R = T[:3, :3]
    t = T[:3, 3]
    Ti = np.eye(4)
    Ti[:3, :3] = R.T
    Ti[:3, 3] = -R.T @ t
    return Ti


# --------------------------------- node ------------------------------------

class HandEye(Node):
    def __init__(self):
        super().__init__('handeye_calibrate')
        self.bridge = CvBridge()
        self.latest_img = None
        self.latest_stamp = None
        self.K = None
        self.D = None
        self.depth_to_color = None      # T: point_depth -> point_color (4x4)

        # ChArUco board + detector
        adict = cv2.aruco.getPredefinedDictionary(ARUCO_DICT)
        self.board = cv2.aruco.CharucoBoard(
            (SQUARES_X, SQUARES_Y), SQUARE_LEN, MARKER_LEN, adict)
        try:
            self.board.setLegacyPattern(LEGACY_PATTERN)
        except Exception:
            self.get_logger().warn('setLegacyPattern unavailable in this OpenCV')
        params = cv2.aruco.CharucoParameters()
        self.detector = cv2.aruco.CharucoDetector(self.board, params)

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.create_subscription(Image, COLOR_TOPIC, self._img_cb,
                                 qos_profile_sensor_data)
        self.create_subscription(CameraInfo, INFO_TOPIC, self._info_cb, 10)
        if HAVE_EXTRINSICS:
            # extrinsics are latched (transient local)
            from rclpy.qos import QoSProfile, QoSDurabilityPolicy
            q = QoSProfile(depth=1)
            q.durability = QoSDurabilityPolicy.TRANSIENT_LOCAL
            self.create_subscription(Extrinsics, EXTR_TOPIC, self._extr_cb, q)

        # storage: lists of 4x4 transforms
        self.T_base_hand = []     # gripper in base   (A side)
        self.T_cam_board = []     # board in camera   (B side)
        self.overlays = []

        os.makedirs(SAVE_DIR, exist_ok=True)
        self.get_logger().info('handeye_calibrate ready.')

    # -- callbacks --
    def _img_cb(self, msg):
        try:
            self.latest_img = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
            self.latest_stamp = msg.header.stamp
        except Exception as e:
            self.get_logger().error(f'cv_bridge: {e}')

    def _info_cb(self, msg):
        self.K = np.array(msg.k, dtype=np.float64).reshape(3, 3)
        self.D = np.array(msg.d, dtype=np.float64).reshape(-1, 1)

    def _extr_cb(self, msg):
        R = np.array(msg.rotation, dtype=np.float64).reshape(3, 3).T  # column-major
        t = np.array(msg.translation, dtype=np.float64)
        self.depth_to_color = make_T(R, t)  # p_color = R p_depth + t

    # -- detection --
    def detect_board_pose(self, img):
        """Return (T_cam_board 4x4, overlay, n_corners) or (None, overlay, n)."""
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        cc, cids, mc, mids = self.detector.detectBoard(gray)
        overlay = img.copy()
        if mids is not None and len(mids) > 0:
            cv2.aruco.drawDetectedMarkers(overlay, mc, mids)
        n = 0 if cids is None else len(cids)
        if n < MIN_CORNERS:
            return None, overlay, n
        cv2.aruco.drawDetectedCornersCharuco(overlay, cc, cids)
        obj_pts, img_pts = self.board.matchImagePoints(cc, cids)
        if obj_pts is None or len(obj_pts) < MIN_CORNERS:
            return None, overlay, n
        ok, rvec, tvec = cv2.solvePnP(
            obj_pts, img_pts, self.K, self.D,
            flags=cv2.SOLVEPNP_ITERATIVE)
        if not ok:
            return None, overlay, n
        # reprojection error
        proj, _ = cv2.projectPoints(obj_pts, rvec, tvec, self.K, self.D)
        err = float(np.mean(np.linalg.norm(
            proj.reshape(-1, 2) - img_pts.reshape(-1, 2), axis=1)))
        cv2.drawFrameAxes(overlay, self.K, self.D, rvec, tvec, SQUARE_LEN * 3)
        R, _ = cv2.Rodrigues(rvec)
        cv2.putText(overlay, f'corners={n} reproj={err:.2f}px',
                    (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)
        return make_T(R, tvec.reshape(3)), overlay, n

    # -- TF --
    def lookup_base_hand(self, stamp):
        for t in (stamp, rclpy.time.Time()):
            try:
                tr = self.tf_buffer.lookup_transform(
                    BASE_FRAME, HAND_FRAME, t,
                    timeout=rclpy.duration.Duration(seconds=0.3))
                q = tr.transform.rotation
                p = tr.transform.translation
                R = quat_to_mat(q.x, q.y, q.z, q.w)
                return make_T(R, [p.x, p.y, p.z])
            except (tf2_ros.LookupException, tf2_ros.ExtrapolationException,
                    tf2_ros.ConnectivityException):
                continue
        return None

    # -- actions --
    def capture(self):
        if self.latest_img is None:
            print('  ! no color image yet'); return
        if self.K is None:
            print('  ! no camera_info yet'); return
        T_cb, overlay, n = self.detect_board_pose(self.latest_img)
        if T_cb is None:
            print(f'  ! board not detected well (corners={n}, need >={MIN_CORNERS})')
            cv2.imwrite(os.path.join(SAVE_DIR, 'last_failed.png'), overlay)
            return
        T_bh = self.lookup_base_hand(self.latest_stamp)
        if T_bh is None:
            print(f'  ! TF {BASE_FRAME}->{HAND_FRAME} unavailable'); return
        self.T_base_hand.append(T_bh)
        self.T_cam_board.append(T_cb)
        self.overlays.append(overlay)
        idx = len(self.T_base_hand)
        cv2.imwrite(os.path.join(SAVE_DIR, f'sample_{idx:02d}.png'), overlay)
        print(f'  + sample {idx} captured (charuco corners={n}). '
              f'board dist={np.linalg.norm(T_cb[:3,3]):.3f} m')
        if idx >= 3:
            print('    (>=3 samples: you can press s to solve; 10-15 recommended)')

    def undo(self):
        if self.T_base_hand:
            self.T_base_hand.pop(); self.T_cam_board.pop(); self.overlays.pop()
            print(f'  - removed last sample, {len(self.T_base_hand)} left')
        else:
            print('  nothing to undo')

    def solve(self, quiet=False):
        n = len(self.T_base_hand)
        if n < 3:
            print(f'  ! need >=3 samples, have {n}'); return None
        R_g2b = [T[:3, :3] for T in self.T_base_hand]
        t_g2b = [T[:3, 3] for T in self.T_base_hand]
        R_t2c = [T[:3, :3] for T in self.T_cam_board]
        t_t2c = [T[:3, 3] for T in self.T_cam_board]

        methods = {
            'TSAI': cv2.CALIB_HAND_EYE_TSAI,
            'PARK': cv2.CALIB_HAND_EYE_PARK,
            'HORAUD': cv2.CALIB_HAND_EYE_HORAUD,
            'DANIILIDIS': cv2.CALIB_HAND_EYE_DANIILIDIS,
        }
        results = {}
        for name, m in methods.items():
            try:
                R, t = cv2.calibrateHandEye(R_g2b, t_g2b, R_t2c, t_t2c, method=m)
                results[name] = make_T(R, t.reshape(3))
            except Exception as e:
                print(f'  {name} failed: {e}')

        if not results:
            print('  ! all methods failed'); return None

        if not quiet:
            print('\n  gripper_base -> camera_color_optical (per method):')
            for name, T in results.items():
                xyz = T[:3, 3]
                rpy = mat_to_rpy(T[:3, :3])
                print(f'    {name:11s} xyz=[{xyz[0]:+.4f} {xyz[1]:+.4f} {xyz[2]:+.4f}]'
                      f'  rpy=[{rpy[0]:+.4f} {rpy[1]:+.4f} {rpy[2]:+.4f}]')

        # Consistency check: spread of translations across methods
        ts = np.array([T[:3, 3] for T in results.values()])
        spread = float(np.max(np.linalg.norm(ts - ts.mean(axis=0), axis=1)))
        self._residual_report(results.get('PARK', list(results.values())[0]))

        T_color = results.get('DANIILIDIS', results.get('PARK'))
        # shift into DEPTH optical frame
        if self.depth_to_color is not None:
            d2c = self.depth_to_color
            extr_note = 'applied live depth_to_color extrinsic'
        else:
            d2c = make_T(np.eye(3), DEFAULT_DEPTH_TO_COLOR_T)
            extr_note = ('used FALLBACK D435i depth_to_color extrinsic '
                         '(live topic unavailable)')
        T_color_depth = inv_T(d2c)  # p_depth from p_color
        T_depth = T_color @ T_color_depth

        xyz = T_depth[:3, 3]
        rpy = mat_to_rpy(T_depth[:3, :3])
        print('\n' + '=' * 66)
        print(f'  RESULT  ({extr_note})')
        print(f'  method inter-agreement (translation spread): {spread*1000:.1f} mm')
        print('  gripper_base -> camera_depth_optical_frame  (xacro joint):')
        print(f'\n    <origin xyz="{xyz[0]:.5f} {xyz[1]:.5f} {xyz[2]:.5f}" '
              f'rpy="{rpy[0]:.5f} {rpy[1]:.5f} {rpy[2]:.5f}"/>\n')
        print('=' * 66)
        self._save_solution(T_depth, xyz, rpy)
        self.last_xyz = xyz
        self.last_rpy = rpy
        return T_depth

    def _residual_report(self, X):
        """AX=XB residual: how well X explains all pose pairs."""
        n = len(self.T_base_hand)
        rot_errs, trn_errs = [], []
        for i in range(n):
            for j in range(i + 1, n):
                A = inv_T(self.T_base_hand[j]) @ self.T_base_hand[i]   # hand motion
                B = self.T_cam_board[j] @ inv_T(self.T_cam_board[i])   # cam motion
                # A X should equal X B
                L = A @ X
                Rr = X @ B
                dR = L[:3, :3].T @ Rr[:3, :3]
                ang = np.degrees(np.arccos(
                    max(-1.0, min(1.0, (np.trace(dR) - 1) / 2))))
                dt = np.linalg.norm(L[:3, 3] - Rr[:3, 3])
                rot_errs.append(ang); trn_errs.append(dt)
        if rot_errs:
            print(f'  AX=XB residual: rot {np.mean(rot_errs):.2f} deg, '
                  f'trans {np.mean(trn_errs)*1000:.1f} mm '
                  f'(lower = better; large -> add more varied orientations)')

    def _save_solution(self, T, xyz, rpy):
        np.savez(os.path.join(SAVE_DIR, 'handeye_result.npz'),
                 T_base_hand=np.array(self.T_base_hand),
                 T_cam_board=np.array(self.T_cam_board),
                 T_gripper_depth=T, xyz=xyz, rpy=rpy)
        with open(os.path.join(SAVE_DIR, 'result.txt'), 'w') as f:
            f.write(f'# {datetime.datetime.now()}  samples={len(self.T_base_hand)}\n')
            f.write(f'<origin xyz="{xyz[0]:.5f} {xyz[1]:.5f} {xyz[2]:.5f}" '
                    f'rpy="{rpy[0]:.5f} {rpy[1]:.5f} {rpy[2]:.5f}"/>\n')
        print(f'  saved to {SAVE_DIR}/')

    def write_xacro(self):
        if not hasattr(self, 'last_xyz'):
            print('  ! solve first (s)'); return
        if not os.path.exists(XACRO):
            print(f'  ! xacro not found: {XACRO}'); return
        with open(XACRO) as f:
            lines = f.readlines()
        out, patched = [], False
        for i, ln in enumerate(lines):
            if ('camera_depth_optical_frame_joint' in
                    ''.join(lines[max(0, i - 3):i + 1])) and '<origin' in ln and not patched:
                indent = ln[:len(ln) - len(ln.lstrip())]
                x, r = self.last_xyz, self.last_rpy
                bak = ln.rstrip('\n')
                out.append(f'{indent}<!-- was: {bak.strip()} -->\n')
                out.append(f'{indent}<origin xyz="{x[0]:.5f} {x[1]:.5f} {x[2]:.5f}" '
                           f'rpy="{r[0]:.5f} {r[1]:.5f} {r[2]:.5f}"/>\n')
                patched = True
            else:
                out.append(ln)
        if patched:
            with open(XACRO, 'w') as f:
                f.writelines(out)
            print(f'  wrote new origin into {XACRO} (old line kept as comment).')
            print('  Rebuild/relaunch robot_state_publisher to apply.')
        else:
            print('  ! could not find camera_depth_optical_frame_joint <origin>')


HELP = ('commands: [Enter/c]=capture  [s]=solve  [u]=undo  '
        '[w]=write xacro  [q]=quit')


def main():
    rclpy.init()
    node = HandEye()
    spin = threading.Thread(
        target=rclpy.spin, args=(node,), daemon=True)
    spin.start()

    print('\n' + HELP + '\n')
    print('Hold the board still. Jog the arm to a pose that sees the board, '
          'let it settle, then capture. Vary wrist ORIENTATION between poses.\n')
    try:
        while rclpy.ok():
            sys.stdout.write('> '); sys.stdout.flush()
            r, _, _ = select.select([sys.stdin], [], [], 0.5)
            if not r:
                continue
            cmd = sys.stdin.readline().strip().lower()
            if cmd in ('', 'c'):
                node.capture()
            elif cmd == 's':
                node.solve()
            elif cmd == 'u':
                node.undo()
            elif cmd == 'w':
                node.write_xacro()
            elif cmd in ('q', 'quit', 'exit'):
                break
            elif cmd in ('h', 'help', '?'):
                print(HELP)
            else:
                print('  ?  ' + HELP)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
