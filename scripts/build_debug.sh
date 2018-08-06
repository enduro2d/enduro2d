#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/debug
cd $BUILD_DIR/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
cmake --build . -- -j8
ctest --verbose
cd ../..
