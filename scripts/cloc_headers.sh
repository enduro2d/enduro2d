#!/bin/bash
SCRIPT_DIR=`dirname "$BASH_SOURCE"`
cloc \
  $SCRIPT_DIR/../headers/enduro2d \
  $SCRIPT_DIR/../headers/3rdparty/ecs.hpp \
  $SCRIPT_DIR/../headers/3rdparty/promise.hpp \
