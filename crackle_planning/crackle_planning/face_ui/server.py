"""Standalone HTTP + Server-Sent-Events server for Crackle's whimsical face UI
and the separate pipeline/debug view.

Serves two independent, self-contained pages that both stream off the same SSE
connection (``/events``) but never interfere with each other:
  * ``/`` (``index.html``)     — the whimsical face, unchanged. Driven by
    "latest value wins" state POSTed to ``/update``.
  * ``/pipeline.html``          — the pipeline/debug view (a separate browser
    tab, see PIPELINE_UI_ARCHITECTURE.md). Driven by append-only events
    POSTed to ``/pipeline``. Existing ``/update`` consumers never see these
    (they're sent as a distinct SSE ``event: pipeline`` frame that a plain
    ``onmessage`` listener — what ``index.html`` uses — ignores).

The planner FSM (``main.py``) and LLM loop (``_llm.py``) push updates via
:mod:`crackle_planning.face_ui.ui_client` (``ui_publish`` / ``emit_stage``).

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
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
INDEX_PATH = os.path.join(HERE, "index.html")
PIPELINE_INDEX_PATH = os.path.join(HERE, "pipeline.html")

DEFAULT_PORT = int(os.environ.get("CRACKLE_UI_PORT", "8137"))

# The set of fields the face UI understands. Anything else in an /update is ignored.
_KNOWN_FIELDS = {"state", "emotion", "transcript", "response", "wakeword"}

# The set of fields a pipeline event (POST /pipeline) may carry. See
# PIPELINE_UI_ARCHITECTURE.md §4 for the event shape.
_PIPELINE_FIELDS = {"turn_id", "seq", "ts", "stage", "status", "step_index", "payload"}

# How many past pipeline events to keep and replay to a freshly-connected
# pipeline.html tab, so opening/refreshing it mid-turn still shows the turn's
# history instead of just whatever happens next.
_PIPELINE_RING_SIZE = 300


class FaceState:
    """Thread-safe holder of the latest face-UI state, the pipeline-event ring
    buffer, and a fan-out to SSE clients for both."""

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
        self._pipeline_events = []  # ring buffer, oldest first
        self._pipeline_seq = 0
        # Each connected browser (either page) gets its own bounded queue of
        # pre-formatted SSE frame strings.
        self._clients = set()

    # ---- coarse /update state (unchanged behavior) --------------------------

    def snapshot(self):
        with self._lock:
            return dict(self._state)

    def update(self, fields):
        """Merge posted fields into the latest state and broadcast to clients."""
        with self._lock:
            for key, value in fields.items():
                if key in _KNOWN_FIELDS:
                    self._state[key] = value
            self._broadcast(self._sse_frame(None, self._state))

    # ---- pipeline events (new) -----------------------------------------------

    def pipeline_snapshot(self):
        with self._lock:
            return list(self._pipeline_events)

    def add_pipeline_event(self, fields):
        """Validate/trim a posted event, append it to the ring buffer, and
        broadcast it to clients as a named ``pipeline`` SSE event."""
        with self._lock:
            self._pipeline_seq += 1
            event = {k: v for k, v in fields.items() if k in _PIPELINE_FIELDS}
            event.setdefault("seq", self._pipeline_seq)
            event.setdefault("ts", time.time())
            self._pipeline_events.append(event)
            if len(self._pipeline_events) > _PIPELINE_RING_SIZE:
                del self._pipeline_events[: -_PIPELINE_RING_SIZE]
            self._broadcast(self._sse_frame("pipeline", event))
            return event

    # ---- shared SSE plumbing -------------------------------------------------

    @staticmethod
    def _sse_frame(event_name, obj):
        payload = json.dumps(obj)
        if event_name:
            return f"event: {event_name}\ndata: {payload}\n\n"
        return f"data: {payload}\n\n"

    def _broadcast(self, frame):
        dead = []
        for client in self._clients:
            try:
                client.put_nowait(frame)
            except queue.Full:
                dead.append(client)
        for client in dead:
            self._clients.discard(client)

    def add_client(self):
        q = queue.Queue(maxsize=64)
        with self._lock:
            self._clients.add(q)
            # Immediately hand the newcomer the current face state so a
            # late/refreshed page shows reality instead of the placeholder...
            q.put_nowait(self._sse_frame(None, self._state))
            # ...and replay the pipeline ring buffer so a freshly-opened
            # pipeline.html tab reconstructs the current/last turn instead of
            # only seeing events emitted after it connected.
            for event in self._pipeline_events:
                q.put_nowait(self._sse_frame("pipeline", event))
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
            self._serve_file(INDEX_PATH)
        elif self.path in ("/pipeline", "/pipeline.html"):
            self._serve_file(PIPELINE_INDEX_PATH)
        elif self.path == "/events":
            self._serve_events()
        elif self.path == "/state":
            self._serve_json(FACE.snapshot())
        elif self.path == "/pipeline-state":
            self._serve_json(FACE.pipeline_snapshot())
        else:
            self.send_error(404, "Not found")

    def do_POST(self):
        if self.path == "/capture_pose":
            self._handle_capture_pose()
        elif self.path == "/update":
            fields = self._read_json_body()
            if fields is None:
                return
            FACE.update(fields)
            self._serve_json({"ok": True})
        elif self.path == "/pipeline":
            fields = self._read_json_body()
            if fields is None:
                return
            event = FACE.add_pipeline_event(fields)
            self._serve_json({"ok": True, "seq": event["seq"]})
        else:
            self.send_error(404, "Not found")

    # --- helpers -------------------------------------------------------------

    def _read_json_body(self):
        try:
            length = int(self.headers.get("Content-Length", 0))
            raw = self.rfile.read(length) if length else b"{}"
            fields = json.loads(raw or b"{}")
            if not isinstance(fields, dict):
                raise ValueError("body must be a JSON object")
        except (ValueError, json.JSONDecodeError) as exc:
            self.send_error(400, f"Bad request body: {exc}")
            return None
        return fields

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

    def _serve_file(self, path):
        try:
            with open(path, "rb") as f:
                body = f.read()
        except OSError:
            self.send_error(500, f"{os.path.basename(path)} missing")
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
                    frame = client.get(timeout=15)  # already a full "data: ...\n\n" / "event: ...\ndata: ...\n\n" frame
                    self.wfile.write(frame.encode("utf-8"))
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
    print(f"[crackle-face-ui] serving pipeline/debug view at {url}/pipeline.html")
    print(f"[crackle-face-ui] POST pipeline events to {url}/pipeline")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n[crackle-face-ui] shutting down")
    finally:
        server.shutdown()


if __name__ == "__main__":
    main()
