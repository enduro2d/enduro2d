#!/bin/bash
set -e
DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"

ROOT_DIR="${DIR}/../.."
BUILD_DIR="${ROOT_DIR}/build/xcode_darwin"

mkdir -p "$BUILD_DIR"
(cd "${BUILD_DIR}" && cmake "${ROOT_DIR}" -G Xcode)
(cd "${BUILD_DIR}" && open "enduro2d.xcodeproj")
