#!/bin/bash
container_name="crackle_humble_runtime"
image="crackle:humble"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if docker ps --filter "name=${container_name}" --filter "status=running" --format "{{.Names}}" | grep -q "^${container_name}$"; then
    echo "Container '${container_name}' is running. Joining..."
    docker exec -it "${container_name}" bash
else
    echo "Container '${container_name}' is not running. Starting via run.bash..."
    bash "$SCRIPT_DIR/run.bash" "${image}"
fi
