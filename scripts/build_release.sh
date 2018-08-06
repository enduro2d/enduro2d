#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/release
cd $BUILD_DIR/release
cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build . -- -j8
ctest --verbose
cd ../..
