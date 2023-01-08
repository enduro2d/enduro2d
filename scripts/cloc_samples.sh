#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"

ROOT_DIR="${DIR}/.."

cloc \
  "${ROOT_DIR}/samples/bin" \
  "${ROOT_DIR}/samples/sources"
