#!/bin/bash
set -e

BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/coverage
pushd $BUILD_DIR/coverage
cmake -DCMAKE_BUILD_TYPE=Debug -DE2D_BUILD_WITH_COVERAGE=ON ../..
cmake --build .

lcov -d . -z
ctest --verbose

lcov -d . -c -o "coverage.info"
lcov -r "coverage.info" "*/usr/*" "*/Xcode.app/*" "*/untests/*" "*/samples/*" "*/3rdparty/*" "*/modules/*" -o "coverage.info"
lcov -l "coverage.info"

bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports"
popd
