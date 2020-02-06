#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/Release
pushd $BUILD_DIR/Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build .
ctest --verbose
popd
