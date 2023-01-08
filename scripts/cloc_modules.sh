#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"

ROOT_DIR="${DIR}/.."

cloc \
  "${ROOT_DIR}/modules/defer.hpp/headers" \
  "${ROOT_DIR}/modules/defer.hpp/untests" \
  "${ROOT_DIR}/modules/ecs.hpp/headers" \
  "${ROOT_DIR}/modules/ecs.hpp/untests" \
  "${ROOT_DIR}/modules/enum.hpp/headers" \
  "${ROOT_DIR}/modules/enum.hpp/untests" \
  "${ROOT_DIR}/modules/flat.hpp/headers" \
  "${ROOT_DIR}/modules/flat.hpp/untests" \
  "${ROOT_DIR}/modules/pnpoly.h" \
  "${ROOT_DIR}/modules/promise.hpp/headers" \
  "${ROOT_DIR}/modules/promise.hpp/untests"
