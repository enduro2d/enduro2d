#!/bin/bash
SCRIPT_DIR=`dirname "$BASH_SOURCE"`
cloc \
  $SCRIPT_DIR/../headers/enduro2d \
  $SCRIPT_DIR/../sources/enduro2d \
  $SCRIPT_DIR/../samples/bin \
  $SCRIPT_DIR/../samples/sources \
  $SCRIPT_DIR/../untests/bin \
  $SCRIPT_DIR/../untests/sources
