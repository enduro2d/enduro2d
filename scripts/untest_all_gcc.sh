#!/bin/bash
set -e
SCRIPT_DIR=`dirname "$BASH_SOURCE"`

$SCRIPT_DIR/build_clear.sh
CC=gcc-8 CXX=g++-8 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=gcc-7 CXX=g++-7 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=gcc-6 CXX=g++-6 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=gcc-5 CXX=g++-5 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
CC=gcc-4.9 CXX=g++-4.9 $SCRIPT_DIR/build_all.sh

$SCRIPT_DIR/build_clear.sh
