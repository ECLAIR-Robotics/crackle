"""Fire-and-forget client the planner FSM uses to push state to the face UI.

Every call is best-effort: if the UI server isn't running (or is slow), the post
is silently dropped and the FSM continues unaffected. Nothing here ever raises.

Usage from main.py:
    from crackle_planning.face_ui import ui_client   # (ROS)  or  from face_ui import ui_client
    ui_client.set_state("listening")
    ui_client.set_transcript("bring me the water bottle")
    ui_client.set_emotion("flirty")
    ui_client.set_response("You betcha!")
"""

import json
import os
import threading
import urllib.request

_PORT = os.environ.get("CRACKLE_UI_PORT", "8137")
_URL = f"http://localhost:{_PORT}/update"
_TIMEOUT = 0.4  # seconds — short so a dead server never stalls the FSM


def _post(payload):
    try:
        req = urllib.request.Request(
            _URL,
            data=json.dumps(payload).encode("utf-8"),
            headers={"Content-Type": "application/json"},
            method="POST",
        )
        urllib.request.urlopen(req, timeout=_TIMEOUT).read()
    except Exception:
        # UI server down / unreachable — the face just keeps showing idle.
        pass


def ui_publish(**fields):
    """Send any subset of {state, emotion, transcript, response, wakeword}.

    Runs on a daemon thread so the caller never blocks on network I/O.
    """
    if not fields:
        return
    threading.Thread(target=_post, args=(fields,), daemon=True).start()


def set_state(state: str):
    """One of: 'idle', 'listening', 'thinking'."""
    ui_publish(state=state)


def set_emotion(emotion):
    ui_publish(emotion=emotion or "neutral")


def set_transcript(text):
    ui_publish(transcript=text or "")


def set_response(text):
    ui_publish(response=text or "")


def set_wakeword(detected: bool = True):
    ui_publish(wakeword=bool(detected))
