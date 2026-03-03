#!/usr/bin/env bash
#
# Typical usage: ./join.bash <container_name>
#

CONTAINER_ID=${1:-crackle_humble_runtime}

xhost +
docker exec --privileged -e DISPLAY=${DISPLAY} -e LINES=`tput lines` -it ${CONTAINER_ID} bash
xhost -
