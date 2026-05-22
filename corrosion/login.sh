#!/usr/bin/env bash

shopt -s failglob
set -euo pipefail

SCRIPT_DIR=$(realpath "$(dirname -- "$0")")
echo "Workspace mounted from $SCRIPT_DIR"

DOCKER=""

if command -v docker &> /dev/null; then
    DOCKER="docker"
elif command -v podman &> /dev/null; then
    DOCKER="podman"
else
    echo "No docker or podman installed"
    exit 1
fi

echo "Using $DOCKER"

DOCKER_IMAGE="${DOCKER_IMAGE:-corrosion-dev-base}"

"$DOCKER" run \
    --platform linux/amd64 \
    --rm \
    -it \
    --name corrosion-dev \
    "$DOCKER_IMAGE" \
    /bin/bash
