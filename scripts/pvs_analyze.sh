#!/bin/bash
set -e
DIR="$( cd "$( dirname "${BASH_SOURCE}" )" && pwd )"

ROOT_DIR="${DIR}/.."
BUILD_DIR="${ROOT_DIR}/build/pvs_analyze"

mkdir -p "${BUILD_DIR}"
(cd "${BUILD_DIR}" && cmake "${ROOT_DIR}" -DCMAKE_EXPORT_COMPILE_COMMANDS=On)
(cd "${BUILD_DIR}" && pvs-studio-analyzer analyze\
                    -e "${ROOT_DIR}/headers/3rdparty"\
                    -e "${ROOT_DIR}/modules"\
                    -e "${ROOT_DIR}/sources/3rdparty"\
                    -e "${ROOT_DIR}/untests"\
                    -o pvs_report.log\
                    -j8)
(cd "${BUILD_DIR}" && rm -rf pvs_report)
(cd "${BUILD_DIR}" && plog-converter -a GA:1,2 -t fullhtml -o pvs_report pvs_report.log)
(cd "${BUILD_DIR}" && open pvs_report/index.html)
