#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/xcode
pushd $BUILD_DIR/xcode
cmake -G Xcode ../..
open enduro2d.xcodeproj
popd
