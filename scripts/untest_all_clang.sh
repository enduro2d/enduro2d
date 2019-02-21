#!/bin/bash
set -e
SCRIPT_DIR=`dirname "$BASH_SOURCE"`

$SCRIPT_DIR/build_clear.sh
CC=clang-6.0 CXX=clang++-6.0 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=clang-5.0 CXX=clang++-5.0 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=clang-4.0 CXX=clang++-4.0 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=clang-3.9 CXX=clang++-3.9 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=clang-3.8 CXX=clang++-3.8 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
