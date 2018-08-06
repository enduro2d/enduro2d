#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/xcode
cd $BUILD_DIR/xcode
cmake -G Xcode ../..
open enduro2d.xcodeproj
