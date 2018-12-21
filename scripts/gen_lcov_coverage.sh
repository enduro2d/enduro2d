#!/bin/bash
set -e

BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/coverage
cd $BUILD_DIR/coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DE2D_BUILD_WITH_COVERAGE=ON ../..
cmake --build . -- -j8
ctest --verbose

lcov -d . -c -o "coverage.info"
lcov -r "coverage.info" "*/Xcode.app/*" "*/untests/*" "*/3rdparty/*" "*/modules/*" -o "coverage-filtered.info"
lcov -d . -z

genhtml -o "html" "coverage-filtered.info"
open html/index.html
