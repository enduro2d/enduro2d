#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/Debug
cd $BUILD_DIR/Debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cmake --build .
ctest --verbose
cd ../..
