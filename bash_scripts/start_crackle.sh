#!/usr/bin/env bash
# Start Crackle stack: face_exec, source env via 'scrackle', then ros2 launch
# Usage:
#   bash start_crackle.sh [simulated] [simulate_vision]

set -Eeuo pipefail

WS_DIR="$HOME/crackle_ws"
FACE_EXEC="$HOME/crackle_face/game/bin/Debug/face_exec"
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

echo "[2/5] Starting whimsical face UI"
UI_PORT="${CRACKLE_UI_PORT:-8137}"
UI_URL="http://localhost:${UI_PORT}"
# The face UI server is a standalone, dependency-free Python process. It shows
# placeholder / idle animations immediately and comes alive once main.py runs
# and starts POSTing state to it. Run it straight from source so it does not
# depend on the workspace being built yet.
CRACKLE_UI_PORT="$UI_PORT" nohup python3 "$WS_DIR/src/crackle/crackle_planning/crackle_planning/face_ui/server.py" \
  >"$LOG_DIR/face_ui.log" 2>&1 &
UI_PID=$!
echo "face UI started with PID $UI_PID at $UI_URL (logs: $LOG_DIR/face_ui.log)"

cleanup() {
  if [[ -n "${UI_PID:-}" ]] && kill -0 "$UI_PID" 2>/dev/null; then
    echo "Stopping face UI (PID $UI_PID)..."
    kill "$UI_PID" 2>/dev/null || true
  fi
}
trap cleanup EXIT INT TERM

# Give the server a moment to bind, then open the face fullscreen. Prefer an
# app/kiosk window; fall back through browsers; finally just print the URL.
sleep 1
open_face() {
  for browser in chromium-browser chromium google-chrome google-chrome-stable brave-browser; do
    if command -v "$browser" >/dev/null 2>&1; then
      nohup "$browser" --app="$UI_URL" --start-fullscreen --no-first-run \
        >"$LOG_DIR/face_ui_browser.log" 2>&1 &
      echo "Opened face UI in $browser (fullscreen app mode)"
      return 0
    fi
  done
  if command -v xdg-open >/dev/null 2>&1; then
    nohup xdg-open "$UI_URL" >"$LOG_DIR/face_ui_browser.log" 2>&1 &
    echo "Opened face UI via xdg-open"
    return 0
  fi
  echo "No browser found — open the face UI manually at: $UI_URL"
}
open_face

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
echo "REMINDER: Launch claw_degree_publisher manually in a separate terminal:"
echo "  ros2 run claw_degree_publisher claw_degree_publisher"
# Do NOT 'exec' so cleanup trap still fires after ros2 exits
ros2 launch crackle_bringup crackle_bringup.launch.py \
  simulated:="$simulated" \
  simulate_vision:="$simulate_vision"
