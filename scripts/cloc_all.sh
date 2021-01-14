#!/bin/bash
SCRIPT_DIR=`dirname "$BASH_SOURCE"`
cloc \
  $SCRIPT_DIR/../headers/enduro2d \
  $SCRIPT_DIR/../sources/enduro2d \
  $SCRIPT_DIR/../samples/bin \
  $SCRIPT_DIR/../samples/sources \
  $SCRIPT_DIR/../untests/bin \
  $SCRIPT_DIR/../untests/sources \
  $SCRIPT_DIR/../modules/defer.hpp/headers \
  $SCRIPT_DIR/../modules/defer.hpp/untests \
  $SCRIPT_DIR/../modules/ecs.hpp/headers \
  $SCRIPT_DIR/../modules/ecs.hpp/untests \
  $SCRIPT_DIR/../modules/enum.hpp/headers \
  $SCRIPT_DIR/../modules/enum.hpp/untests \
  $SCRIPT_DIR/../modules/flat.hpp/headers \
  $SCRIPT_DIR/../modules/flat.hpp/untests \
  $SCRIPT_DIR/../modules/pnpoly.h \
  $SCRIPT_DIR/../modules/promise.hpp/headers \
  $SCRIPT_DIR/../modules/promise.hpp/untests
