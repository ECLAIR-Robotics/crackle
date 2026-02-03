#!/usr/bin/env bash
# Start Crackle stack: face_exec, source env via 'scrackle', then ros2 launch
# Usage:
#   bash start_crackle.sh [simulated] [simulate_vision]

set -Eeuo pipefail

# --- Determine workspace root and directories ---
# SCRIPT_DIR = directory of this script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &>/dev/null && pwd )"
# WS_DIR = crackle_ws root (3 levels up from bash_scripts)
WS_DIR="$( dirname "$( dirname "$( dirname "$SCRIPT_DIR" )" )" )"
# CRACKLE_FACE_DIR = sibling to crackle_ws
CRACKLE_FACE_DIR="$( dirname "$WS_DIR" )/crackle_face"
FACE_EXEC="$CRACKLE_FACE_DIR/game/bin/Debug/face_exec"
LOG_DIR="$WS_DIR/logs"

# --- Parse flags ---
simulated=false
simulate_vision=false
for arg in "$@"; do
  case "$arg" in
    simulated) simulated=true ;;
    simulate_vision) simulate_vision=true ;;
    *) echo "WARN: Ignoring unknown arg: '$arg'" >&2 ;;
  esac
done

echo "[1/6] Ensuring workspace directory: $WS_DIR"
[[ -d "$WS_DIR" ]] || { echo "ERROR: $WS_DIR not found"; exit 1; }

mkdir -p "$LOG_DIR"

# --- Check crackle_face ---
if [[ ! -d "$CRACKLE_FACE_DIR" ]]; then
  echo "ERROR: crackle_face directory not found at $CRACKLE_FACE_DIR"
  echo "Please clone it using:"
  echo "  git clone https://github.com/ECLAIR-Robotics/crackle_face.git $CRACKLE_FACE_DIR"
  exit 1
fi

echo "[2/6] Starting face executable: $FACE_EXEC"
[[ -x "$FACE_EXEC" ]] || { echo "ERROR: face_exec missing or not executable: $FACE_EXEC"; exit 1; }
nohup "$FACE_EXEC" >"$LOG_DIR/face_exec.log" 2>&1 &
FACE_PID=$!
echo "face_exec started with PID $FACE_PID (logs: $LOG_DIR/face_exec.log)"

cleanup() {
  if kill -0 "$FACE_PID" 2>/dev/null; then
    echo "Stopping face_exec (PID $FACE_PID)..."
    kill "$FACE_PID" 2>/dev/null || true
  fi
}
trap cleanup EXIT INT TERM

echo "[3/6] Loading shell rc (aliases)"
shopt -s expand_aliases || true
[[ -f "$HOME/.bashrc" ]] && source "$HOME/.bashrc"
[[ -z "$(alias scrackle 2>/dev/null || true)" && -f "$HOME/.zshrc" ]] && source "$HOME/.zshrc" || true

safe_source() {
  local file="$1"
  if [[ -f "$file" ]]; then
    local had_nounset=0
    if [[ -o nounset ]]; then had_nounset=1; set +u; fi
    export COLCON_TRACE="${COLCON_TRACE:-0}"
    # shellcheck disable=SC1090
    source "$file"
    (( had_nounset )) && set -u
    return 0
  fi
  return 1
}

echo "[4/6] Sourcing environment"
if alias scrackle >/dev/null 2>&1; then
  scrackle
else
  echo "WARN: 'scrackle' alias not found. Falling back to $WS_DIR/install/setup.bash"
  safe_source "$WS_DIR/install/setup.bash" || {
    echo "ERROR: Could not source ROS 2 env (missing $WS_DIR/install/setup.bash)"; exit 1;
  }
fi

echo "[5/6] Launching ROS 2 bringup with args: simulated:=$simulated simulate_vision:=$simulate_vision"
ros2 launch crackle_bringup crackle_bringup.launch.py \
  simulated:="$simulated" \
  simulate_vision:="$simulate_vision"

echo "[6/6] Crackle stack started successfully."
