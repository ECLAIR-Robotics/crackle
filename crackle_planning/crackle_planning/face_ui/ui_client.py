"""Fire-and-forget client the planner FSM uses to push state to the face UI.

Every call is best-effort: if the UI server isn't running (or is slow), the post
is silently dropped and the FSM continues unaffected. Nothing here ever raises.

Usage from main.py:
    from crackle_planning.face_ui import ui_client   # (ROS)  or  from face_ui import ui_client
    ui_client.set_state("listening")
    ui_client.set_transcript("bring me the water bottle")
    ui_client.set_emotion("flirty")
    ui_client.set_response("You betcha!")

Usage from _llm.py (pipeline/debug view — see PIPELINE_UI_ARCHITECTURE.md):
    ui_client.emit_stage("llm_plan", "start", turn_id=turn_id, prompt=prompt)
    ui_client.emit_stage("api:openai_responses", "loading", turn_id=turn_id)
"""

import json
import os
import threading
import urllib.request

_PORT = os.environ.get("CRACKLE_UI_PORT", "8137")
_BASE = f"http://localhost:{_PORT}"
_TIMEOUT = 0.4  # seconds — short so a dead server never stalls the FSM


def _post(path, body):
    try:
        req = urllib.request.Request(
            f"{_BASE}{path}",
            data=json.dumps(body).encode("utf-8"),
            headers={"Content-Type": "application/json"},
            method="POST",
        )
        urllib.request.urlopen(req, timeout=_TIMEOUT).read()
    except Exception:
        # UI server down / unreachable — the UI just keeps showing stale state.
        pass


def ui_publish(**fields):
    """Send any subset of {state, emotion, transcript, response, wakeword}.

    Runs on a daemon thread so the caller never blocks on network I/O.
    """
    if not fields:
        return
    threading.Thread(target=_post, args=("/update", fields), daemon=True).start()


def emit_stage(stage: str, status: str, turn_id: str = None, step_index: int = None, **payload):
    """Push one pipeline event to the standalone pipeline/debug view (see
    PIPELINE_UI_ARCHITECTURE.md §4). Distinct from ``ui_publish`` — this is an
    append-only event, not a "latest value wins" field update.

    ``stage`` should be one of the ids from §4.1 (e.g. "llm_plan",
    "api:openai_responses", "api:elevenlabs_tts", "face", "tool:pick_up").
    ``status`` is one of "start" | "loading" | "data" | "done" | "error".
    Any extra keyword arguments become ``payload`` on the event.

    Runs on a daemon thread, like ``ui_publish`` — never blocks or raises.
    """
    body = {"stage": stage, "status": status}
    if turn_id is not None:
        body["turn_id"] = turn_id
    if step_index is not None:
        body["step_index"] = step_index
    if payload:
        body["payload"] = payload
    threading.Thread(target=_post, args=("/pipeline", body), daemon=True).start()


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
