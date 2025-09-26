#!/usr/bin/env bash
set -euo pipefail

BAGS_ROOT="$HOME/crackle_ws/src/crackle_rosbag/rosbags"
BAG_NAME="latest_crackle_rosbag"
BAG_PATH="$BAGS_ROOT/$BAG_NAME"

usage() {
  cat <<EOF
Usage: $(basename "$0") [-r | -p]

  -r    Rotate existing '$BAG_NAME' (if present) to '${BAG_NAME}_<N>'
        where N = highest existing suffix + 1, then record all topics to:
        $BAG_PATH
        (Ctrl-C to stop recording)

  -p    Play the bag on loop using:
        ros2 bag play $BAG_NAME --loop
EOF
}

rotate_latest_if_needed() {
  mkdir -p "$BAGS_ROOT"

  # Find highest existing suffix among latest_crackle_rosbag and *_<num>
  local max_suffix=0
  shopt -s nullglob
  for d in "$BAGS_ROOT"/${BAG_NAME}*; do
    [[ -d "$d" ]] || continue
    base="$(basename "$d")"
    if [[ "$base" == "$BAG_NAME" ]]; then
      # Treat unsuffixed as suffix 0
      (( max_suffix = max_suffix < 0 ? 0 : max_suffix ))
    elif [[ "$base" =~ ^${BAG_NAME}_([0-9]+)$ ]]; then
      num="${BASH_REMATCH[1]}"
      (( num > max_suffix )) && max_suffix="$num"
    fi
  done
  shopt -u nullglob

  # If the base bag exists, rename it to next suffix
  if [[ -d "$BAG_PATH" ]]; then
    local next=$((max_suffix + 1))
    local new_path="$BAGS_ROOT/${BAG_NAME}_${next}"
    echo "Found existing '$BAG_NAME'. Rotating to: $(basename "$new_path")"
    mv "$BAG_PATH" "$new_path"
  fi
}

if [[ $# -ne 1 ]]; then
  usage
  exit 1
fi

case "$1" in
  -r)
    rotate_latest_if_needed
    echo "Recording all topics to: $BAG_PATH"
    echo "Press Ctrl-C to stop recording..."
    ros2 bag record -o "$BAG_PATH" -a
    ;;
  -p)
    if [[ ! -d "$BAG_PATH" ]]; then
      echo "Error: bag directory not found: $BAG_PATH" >&2
      echo "Hint: record one first with: $(basename "$0") -r" >&2
      exit 2
    fi
    cd "$BAGS_ROOT"
    echo "Playing: ros2 bag play $BAG_NAME --loop"
    ros2 bag play "$BAG_NAME" --loop
    ;;
  -h|--help)
    usage
    ;;
  *)
    echo "Unknown option: $1" >&2
    usage
    exit 1
    ;;
esac
