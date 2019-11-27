#!/bin/bash
set -e
BUILD_DIR=`dirname "$BASH_SOURCE"`/../build
mkdir -p $BUILD_DIR/pvs_analyze
cd $BUILD_DIR/pvs_analyze
rm -rf pvs_report
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On ../..
pvs-studio-analyzer analyze\
                    --disableLicenseExpirationCheck\
                    -e ../../untests\
                    -e ../../modules\
                    -e ../../headers/3rdparty\
                    -e ../../sources/3rdparty\
                    -o pvs_report.log\
                    -j8
plog-converter -a GA:1,2 -t fullhtml -o pvs_report pvs_report.log
open pvs_report/index.html
