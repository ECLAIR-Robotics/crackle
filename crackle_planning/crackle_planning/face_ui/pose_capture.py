"""Optional ROS bridge for the face UI's "capture pose" button.

The face UI server (server.py) is deliberately ROS-free stdlib. This module is
imported lazily ONLY when the capture-pose endpoint is hit, so the UI still runs
standalone (e.g. in dev with no ROS sourced) — in that case the import simply
fails and the button reports the bridge is unavailable.

It calls the ``crackle_manipulation/get_scan_pose`` std_srvs/Trigger service,
which returns the arm's current end-effector pose already formatted as a
scan_poses.json entry (position in metres, rpy in degrees). Copy that string
straight into search_poses.json / idle_scan_poses.json.

Robustness notes (each learned from a real failure):
  * Use the DEFAULT rclpy context. A freshly-constructed private Context() does
    NOT participate in ROS-graph discovery here — wait_for_service always timed
    out even though the service was advertised.
  * Keep ONE persistent node (created once), so graph discovery stays warm and
    the context stays valid across button presses ("rcl node's context is
    invalid" came from re-initialising / stale contexts).
  * Give the node a unique (PID-suffixed) name so it can never collide with a
    node left over from a previous run of this server.
  * Drive the call with spin_until_future_complete under a lock (single spinner,
    never two threads spinning the same node — the ThreadingHTTPServer hands each
    request to its own thread).
  * Use a generous timeout: get_scan_pose waits for the arm's current state,
    which can take a few seconds when the state monitor is cold.
"""

import os
import threading

# Serialize captures so only one request drives/spins the node at a time.
_lock = threading.Lock()

# Persistent rclpy plumbing, created once by _ensure() under _lock.
_state = {"node": None, "client": None}

# Absolute name so resolution never depends on the node's namespace.
SERVICE_NAME = "/crackle_manipulation/get_scan_pose"


def _ensure():
    """Lazily create the persistent node + client (once). Raises on failure."""
    if _state["node"] is not None:
        return
    import rclpy
    from std_srvs.srv import Trigger

    if not rclpy.ok():
        rclpy.init()
    node = rclpy.create_node(f"face_ui_pose_capture_{os.getpid()}")
    client = node.create_client(Trigger, SERVICE_NAME)
    _state.update(node=node, client=client)


def capture_pose(timeout=15.0):
    """Call get_scan_pose and return (ok: bool, message: str).

    ``message`` is the ready-to-paste scan-pose JSON on success, or a human
    readable reason on failure. Never raises.
    """
    with _lock:
        try:
            import rclpy
            from std_srvs.srv import Trigger

            _ensure()
            node, client = _state["node"], _state["client"]

            if not client.wait_for_service(timeout_sec=5.0):
                return False, (
                    f"'{SERVICE_NAME}' unavailable — is the manipulation node "
                    "running?"
                )

            future = client.call_async(Trigger.Request())
            rclpy.spin_until_future_complete(node, future, timeout_sec=timeout)
            if not future.done():
                return False, "timed out waiting for get_scan_pose"

            resp = future.result()
            return bool(resp.success), resp.message or ""
        except Exception as exc:  # ROS not sourced, import error, etc.
            return False, f"pose capture bridge unavailable: {exc}"
