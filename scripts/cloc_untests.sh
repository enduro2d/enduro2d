#!/bin/bash
SCRIPT_DIR=`dirname "$BASH_SOURCE"`
cloc \
  $SCRIPT_DIR/../untests/bin \
  $SCRIPT_DIR/../untests/sources
