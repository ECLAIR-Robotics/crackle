#!/usr/bin/env bash
# Start Crackle stack: face_exec, source env via 'scrackle', then ros2 launch
# Usage:
#   bash start_crackle.sh [simulated] [simulate_vision]

set -Eeuo pipefail

WS_DIR="/home/tanay/crackle_ws"
FACE_EXEC="/home/tanay/crackle_face/game/bin/Debug/face_exec"
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

echo "[1/5] Ensuring workspace directory: $WS_DIR"
[[ -d "$WS_DIR" ]] || { echo "ERROR: $WS_DIR not found"; exit 1; }
cd "$WS_DIR"

mkdir -p "$LOG_DIR"

echo "[2/5] Starting face executable: $FACE_EXEC"
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

echo "[3/5] Loading shell rc (aliases)"
shopt -s expand_aliases || true
[[ -f "$HOME/.bashrc" ]] && source "$HOME/.bashrc"
# If you keep 'scrackle' in zsh, try to import it:
[[ -z "$(alias scrackle 2>/dev/null || true)" && -f "$HOME/.zshrc" ]] && source "$HOME/.zshrc" || true

# helper to safely source files that don't play nice with 'set -u'
safe_source() {
  local file="$1"
  if [[ -f "$file" ]]; then
    local had_nounset=0
    if [[ -o nounset ]]; then had_nounset=1; set +u; fi
    # Avoid COLCON_TRACE 'unbound variable' under nounset
    export COLCON_TRACE="${COLCON_TRACE:-0}"
    # shellcheck disable=SC1090
    source "$file"
    (( had_nounset )) && set -u
    return 0
  fi
  return 1
}

echo "[4/5] Sourcing environment"
if alias scrackle >/dev/null 2>&1; then
  # Call alias inside a subshell that has aliases enabled
  scrackle
else
  echo "WARN: 'scrackle' alias not found. Falling back to $WS_DIR/install/setup.bash"
  safe_source "$WS_DIR/install/setup.bash" || {
    echo "ERROR: Could not source ROS 2 env (missing $WS_DIR/install/setup.bash)"; exit 1;
  }
fi

echo "[5/5] Launching ROS 2 bringup with args: simulated:=$simulated simulate_vision:=$simulate_vision"
# Do NOT 'exec' so cleanup trap still fires after ros2 exits
ros2 launch crackle_bringup crackle_bringup.launch.py \
  simulated:="$simulated" \
  simulate_vision:="$simulate_vision"
