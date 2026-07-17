"""Runtime feature flags for Crackle, loaded from crackle_config.json.

Flags are read fresh from disk on every call to ``load_config`` so that hand
edits to the JSON take effect without rebuilding or (in most cases) restarting.
Loading never raises: a missing or malformed file falls back to DEFAULTS.
"""

import json
import os

# Prefer the source tree so hand edits take effect and the colcon install space
# (which doesn't ship this data file) still finds it; fall back to the pkg dir.
_CONFIG_SRC = os.path.expanduser(
    "~/crackle_ws/src/crackle/crackle_planning/crackle_planning/crackle_config.json"
)
CONFIG_FILE = (
    _CONFIG_SRC
    if os.path.exists(_CONFIG_SRC)
    else os.path.join(os.path.dirname(__file__), "crackle_config.json")
)

# Every supported flag and its default. Only keys listed here are honored.
DEFAULTS = {
    "idle_scanning_enabled": True,
    "look_at_mode": "audio",  # "audio" | "fixed"
    "fixed_look_direction": {"x": 0.3, "y": 0.0, "z": 0.55},
    # Geometry of the hand_object_to_person handover reach (link_base frame).
    # radius: EEF distance from the base for the initial "present" pose.
    # extend: extra distance pushed outward for the final "offer" pose.
    # height: EEF height. rpy (degrees): gripper orientation; yaw is auto-aimed
    # toward the user and this yaw is added as an offset — tune r/p for your gripper.
    "handover": {
        "radius": 0.35,
        "extend": 0.12,
        "height": 0.45,
        "rpy": {"r": 180.0, "p": 0.0, "y": 0.0},
    },
    # Geometry of the fist_bump / high_five greeting reaches (link_base frame).
    # radius: EEF distance from base for the reach. jab: forward nudge for the
    # fist bump. bump_height / five_height: EEF heights. rpy (degrees): gripper
    # orientation; yaw is auto-aimed toward the user and this yaw is added as an
    # offset — tune r/p for your gripper.
    "greeting": {
        "radius": 0.34,
        "jab": 0.07,
        "bump_height": 0.35,
        "five_height": 0.5,
        "rpy": {"r": 180.0, "p": 0.0, "y": 0.0},
    },
}


def load_config() -> dict:
    """Return the feature flags, merged over DEFAULTS. Never raises."""
    cfg = dict(DEFAULTS)
    try:
        with open(CONFIG_FILE, "r") as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"[config] {CONFIG_FILE} not found; using defaults.")
        return cfg
    except (json.JSONDecodeError, ValueError) as exc:
        print(f"[config] failed to parse {CONFIG_FILE}: {exc}; using defaults.")
        return cfg

    if isinstance(data, dict):
        for key in DEFAULTS:
            if key in data:
                cfg[key] = data[key]
    return cfg
