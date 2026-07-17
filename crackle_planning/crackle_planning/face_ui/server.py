"""Standalone HTTP + Server-Sent-Events server for Crackle's whimsical face UI.

Serves a single self-contained animated face page and streams live state to the
browser over SSE. The planner FSM (``main.py``) pushes updates by POSTing JSON to
``/update`` via :mod:`crackle_planning.face_ui.ui_client`.

Design goals:
  * **Zero dependencies** — Python standard library only.
  * **Launch-order independent** — the server can run for ages showing idle
    placeholders; it comes alive the instant the FSM POSTs its first update.
  * **Never blocks / never crashes the caller** — the FSM's posts are
    best-effort; this server just keeps the latest state and fans it out.

Run standalone:
    python3 -m crackle_planning.face_ui.server
    (respects $CRACKLE_UI_PORT, default 8137)
"""

import json
import os
import queue
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
INDEX_PATH = os.path.join(HERE, "index.html")

DEFAULT_PORT = int(os.environ.get("CRACKLE_UI_PORT", "8137"))

# The set of fields the UI understands. Anything else in an /update is ignored.
_KNOWN_FIELDS = {"state", "emotion", "transcript", "response", "wakeword"}


class FaceState:
    """Thread-safe holder of the latest UI state plus a fan-out to SSE clients."""

    def __init__(self):
        self._lock = threading.Lock()
        # Sensible placeholder state shown before the FSM ever posts anything.
        self._state = {
            "state": "idle",
            "emotion": "neutral",
            "transcript": "",
            "response": "",
            "wakeword": False,
        }
        # Each connected browser gets its own bounded queue of JSON strings.
        self._clients = set()

    def snapshot(self):
        with self._lock:
            return dict(self._state)

    def update(self, fields):
        """Merge posted fields into the latest state and broadcast to clients."""
        with self._lock:
            for key, value in fields.items():
                if key in _KNOWN_FIELDS:
                    self._state[key] = value
            payload = json.dumps(self._state)
            dead = []
            for client in self._clients:
                try:
                    client.put_nowait(payload)
                except queue.Full:
                    dead.append(client)
            for client in dead:
                self._clients.discard(client)

    def add_client(self):
        q = queue.Queue(maxsize=32)
        with self._lock:
            self._clients.add(q)
            # Immediately hand the newcomer the current state so a late/refreshed
            # page shows reality instead of the built-in placeholder.
            q.put_nowait(json.dumps(self._state))
        return q

    def remove_client(self, q):
        with self._lock:
            self._clients.discard(q)


FACE = FaceState()


class Handler(BaseHTTPRequestHandler):
    # Quieter logs — one line per request is noisy for an SSE server.
    def log_message(self, *args):
        pass

    def do_GET(self):
        if self.path in ("/", "/index.html"):
            self._serve_index()
        elif self.path == "/events":
            self._serve_events()
        elif self.path == "/state":
            self._serve_json(FACE.snapshot())
        else:
            self.send_error(404, "Not found")

    def do_POST(self):
        if self.path == "/capture_pose":
            self._handle_capture_pose()
            return
        if self.path != "/update":
            self.send_error(404, "Not found")
            return
        try:
            length = int(self.headers.get("Content-Length", 0))
            raw = self.rfile.read(length) if length else b"{}"
            fields = json.loads(raw or b"{}")
            if not isinstance(fields, dict):
                raise ValueError("body must be a JSON object")
        except (ValueError, json.JSONDecodeError) as exc:
            self.send_error(400, f"Bad update: {exc}")
            return
        FACE.update(fields)
        self._serve_json({"ok": True})

    def _handle_capture_pose(self):
        """Capture the arm's current pose via the ROS bridge (best-effort).

        Lazily imports the ROS bridge so the face UI still runs with no ROS
        environment — if the bridge is unavailable the button just reports it.
        """
        try:
            # Works both as an installed module and when server.py is run as a
            # bare script (pose_capture.py sits next to it on sys.path).
            try:
                from crackle_planning.face_ui import pose_capture
            except ImportError:
                import pose_capture
            ok, message = pose_capture.capture_pose()
        except Exception as exc:  # noqa: BLE001 — never let the UI crash
            ok, message = False, f"pose capture bridge unavailable: {exc}"
        self._serve_json({"ok": ok, "message": message})

    # --- helpers -------------------------------------------------------------

    def _serve_index(self):
        try:
            with open(INDEX_PATH, "rb") as f:
                body = f.read()
        except OSError:
            self.send_error(500, "index.html missing")
            return
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _serve_json(self, obj):
        body = json.dumps(obj).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _serve_events(self):
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream")
        self.send_header("Cache-Control", "no-cache")
        self.send_header("Connection", "keep-alive")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()

        client = FACE.add_client()
        try:
            while True:
                try:
                    payload = client.get(timeout=15)
                    self.wfile.write(f"data: {payload}\n\n".encode("utf-8"))
                except queue.Empty:
                    # Comment line keeps the connection (and proxies) alive.
                    self.wfile.write(b": keep-alive\n\n")
                self.wfile.flush()
        except (BrokenPipeError, ConnectionResetError, ValueError):
            pass  # Browser navigated away / closed the tab.
        finally:
            FACE.remove_client(client)


def main():
    port = DEFAULT_PORT
    server = ThreadingHTTPServer(("0.0.0.0", port), Handler)
    server.daemon_threads = True
    url = f"http://localhost:{port}"
    print(f"[crackle-face-ui] serving whimsical face at {url}")
    print(f"[crackle-face-ui] POST updates to {url}/update  (SSE at {url}/events)")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n[crackle-face-ui] shutting down")
    finally:
        server.shutdown()


if __name__ == "__main__":
    main()
