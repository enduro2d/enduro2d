#!/bin/bash
ROOT_DIR=`dirname "$BASH_SOURCE"`/..
MODULES_DIR=$ROOT_DIR/modules
HEADERS_DIR=$ROOT_DIR/headers
SOURCES_DIR=$ROOT_DIR/sources
UNTESTS_DIR=$ROOT_DIR/untests
HEADERS_RDPARTY_DIR=$HEADERS_DIR/3rdparty
SOURCES_RDPARTY_DIR=$SOURCES_DIR/3rdparty

mkdir -p $MODULES_DIR
mkdir -p $HEADERS_DIR
mkdir -p $SOURCES_DIR
mkdir -p $UNTESTS_DIR
mkdir -p $HEADERS_RDPARTY_DIR
mkdir -p $SOURCES_RDPARTY_DIR

git submodule init
git submodule update
git pull --recurse-submodules
git submodule update --init --remote --recursive

mkdir -p $UNTESTS_DIR/catch
cp -fv $MODULES_DIR/catch2/single_include/catch2/catch.hpp $UNTESTS_DIR/catch/catch.hpp

mkdir -p $HEADERS_RDPARTY_DIR/promise.hpp
cp -rfv $MODULES_DIR/promise.hpp/invoke.hpp $HEADERS_RDPARTY_DIR/promise.hpp/invoke.hpp
cp -rfv $MODULES_DIR/promise.hpp/jobber.hpp $HEADERS_RDPARTY_DIR/promise.hpp/jobber.hpp
cp -rfv $MODULES_DIR/promise.hpp/promise.hpp $HEADERS_RDPARTY_DIR/promise.hpp/promise.hpp
cp -rfv $MODULES_DIR/promise.hpp/scheduler.hpp $HEADERS_RDPARTY_DIR/promise.hpp/scheduler.hpp

mkdir -p $SOURCES_RDPARTY_DIR/pugixml
cp -rfv $MODULES_DIR/pugixml/src/. $SOURCES_RDPARTY_DIR/pugixml/

mkdir -p $SOURCES_RDPARTY_DIR/rapidjson
cp -rfv $MODULES_DIR/rapidjson/include/rapidjson/. $SOURCES_RDPARTY_DIR/rapidjson

mkdir -p $SOURCES_RDPARTY_DIR/stb
cp -fv $MODULES_DIR/stb/stb_image.h $SOURCES_RDPARTY_DIR/stb/stb_image.h
cp -fv $MODULES_DIR/stb/stb_image_resize.h $SOURCES_RDPARTY_DIR/stb/stb_image_resize.h
cp -fv $MODULES_DIR/stb/stb_image_write.h $SOURCES_RDPARTY_DIR/stb/stb_image_write.h
cp -fv $MODULES_DIR/stb/stb_rect_pack.h $SOURCES_RDPARTY_DIR/stb/stb_rect_pack.h
cp -fv $MODULES_DIR/stb/stb_sprintf.h $SOURCES_RDPARTY_DIR/stb/stb_sprintf.h
cp -fv $MODULES_DIR/stb/stb_truetype.h $SOURCES_RDPARTY_DIR/stb/stb_truetype.h

mkdir -p $SOURCES_RDPARTY_DIR/utfcpp
cp -rfv $MODULES_DIR/utfcpp/source/. $SOURCES_RDPARTY_DIR/utfcpp/

mkdir -p $HEADERS_RDPARTY_DIR/variant
cp -rfv $MODULES_DIR/variant/include/mpark/. $HEADERS_RDPARTY_DIR/variant/
