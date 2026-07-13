"""Host-side driver for the CRACKLE_CLAW firmware.

This module is the ROS-package "library" view of the CRACKLE_CLAW firmware
(clawArmPositionCalculator/CRACKLE_CLAW, a git submodule). It mirrors the
serial protocol implemented in CRACKLE_Claw/src/main.cpp and exposes a small
Python API (open / close / current angle) so ROS nodes don't have to know the
wire format.

Serial protocol (must stay in sync with the firmware main.cpp):
  * baud rate: 115200
  * commands (host -> ESP32): single char '1' = close, '0' = open
  * telemetry (ESP32 -> host): lines of the form "ANGLE <deg>\\n"
  * completion  (ESP32 -> host): "DONE 1\\n" / "DONE 0\\n" once a close/open
    command has fully finished (grip_object() is blocking), so callers can
    wait until the gripper is actually closed before proceeding.

The firmware drives both pinion servos symmetrically between MIN_ANGLE
(fully open) and MAX_ANGLE (fully closed); see gripper.cpp.
"""

from collections import namedtuple

import serial
from serial.tools import list_ports

# Result of draining the serial port once. ``angle`` is the latest servo angle
# in degrees (or None if no new reading), ``done`` is '1'/'0' if a completion
# ack was seen this poll (or None).
PollResult = namedtuple("PollResult", ["angle", "done"])

# --- Constants mirrored from the CRACKLE_CLAW firmware -----------------------
BAUD_RATE = 115200

# Servo travel, from gripper.cpp (MIN_ANGLE / MAX_ANGLE).
MIN_ANGLE = 0
MAX_ANGLE = 180

# Wire tokens.
CMD_CLOSE = b"1"
CMD_OPEN = b"0"
ANGLE_PREFIX = "ANGLE"
DONE_PREFIX = "DONE"

# USB-serial adapter used by the ESP32 devkit.
DEVICE_DESCRIPTION = "CP2102 USB to UART Bridge Controller"


class CrackleClaw:
    """Serial driver mirroring the firmware's Gripper interface.

    All I/O is best-effort and non-raising: on a disconnect the driver drops
    the port and transparently reconnects on the next call, so a node can keep
    spinning while the ESP32 is unplugged or resetting.
    """

    def __init__(self, logger=None):
        self._logger = logger
        self._ser = None
        self._last_angle = MIN_ANGLE
        self.connect()

    # -- logging helpers ------------------------------------------------------
    def _info(self, msg):
        if self._logger:
            self._logger.info(msg)

    def _warn(self, msg):
        if self._logger:
            self._logger.warn(msg)

    # -- connection management ------------------------------------------------
    @property
    def connected(self):
        return self._ser is not None

    def connect(self):
        """(Re)connect to the ESP32. Safe to call repeatedly."""
        if self._ser is not None:
            return True

        port = None
        for p in list(list_ports.comports()):
            if p.description and DEVICE_DESCRIPTION in p.description.strip():
                port = p.device
                break

        if port is None:
            self._warn("CRACKLE_CLAW ESP32 not found — will retry on next command")
            return False

        try:
            self._ser = serial.Serial(port, baudrate=BAUD_RATE, timeout=0)
            self._info(f"Connected to CRACKLE_CLAW on {port}")
            return True
        except (OSError, serial.SerialException) as exc:
            self._warn(f"Failed to open {port}: {exc}")
            self._ser = None
            return False

    def _drop(self, reason):
        self._warn(f"Serial error ({reason}) — dropping port, will reconnect")
        try:
            if self._ser is not None:
                self._ser.close()
        except (OSError, serial.SerialException):
            pass
        self._ser = None

    # -- commands (mirror Gripper: grip_object / release) ---------------------
    def _write(self, payload):
        if self._ser is None and not self.connect():
            return False
        try:
            self._ser.write(payload)
            return True
        except (OSError, serial.SerialException) as exc:
            self._drop(str(exc))
            return False

    def close(self):
        """Close the gripper (firmware: release() then grip_object())."""
        return self._write(CMD_CLOSE)

    def open(self):
        """Open the gripper (firmware: release())."""
        return self._write(CMD_OPEN)

    # -- telemetry ------------------------------------------------------------
    def poll(self):
        """Drain pending serial lines. Returns a PollResult with the latest
        servo angle (or None) and the last completion ack seen (or None)."""
        if self._ser is None:
            return PollResult(None, None)

        latest_angle = None
        latest_done = None
        try:
            while self._ser.in_waiting > 0:
                line = self._ser.readline().decode("utf-8", errors="ignore").strip()
                parts = line.split()
                if len(parts) != 2:
                    continue
                if parts[0] == ANGLE_PREFIX:
                    try:
                        latest_angle = int(parts[1])
                    except ValueError:
                        continue
                elif parts[0] == DONE_PREFIX and parts[1] in ("0", "1"):
                    latest_done = parts[1]
        except (OSError, serial.SerialException) as exc:
            self._drop(str(exc))
            return PollResult(None, None)

        if latest_angle is not None:
            self._last_angle = max(MIN_ANGLE, min(MAX_ANGLE, latest_angle))
        angle = self._last_angle if latest_angle is not None else None
        return PollResult(angle, latest_done)

    def read_angle(self):
        """Backwards-compatible helper: drain serial and return the latest
        servo angle in degrees, or None if no new reading was available."""
        return self.poll().angle

    @property
    def last_angle(self):
        return self._last_angle

    @staticmethod
    def angle_to_fraction(angle):
        """Map a servo angle to a 0.0 (open) .. 1.0 (closed) fraction."""
        span = MAX_ANGLE - MIN_ANGLE
        if span == 0:
            return 0.0
        return max(0.0, min(1.0, (angle - MIN_ANGLE) / span))
