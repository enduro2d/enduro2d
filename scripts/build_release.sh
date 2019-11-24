#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/Release
cd $BUILD_DIR/Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build .
ctest --verbose
cd ../..
