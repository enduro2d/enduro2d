#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"

ROOT_DIR="${DIR}/.."

cloc \
  "${ROOT_DIR}/untests/bin" \
  "${ROOT_DIR}/untests/sources"
