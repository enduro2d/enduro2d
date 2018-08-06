#!/bin/bash
set -e
SCRIPT_DIR=`dirname "$BASH_SOURCE"`
$SCRIPT_DIR/build_debug.sh
$SCRIPT_DIR/build_release.sh
