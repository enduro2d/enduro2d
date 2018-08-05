#!/bin/bash
ROOT_DIR=`dirname "$BASH_SOURCE"`/..
MODULES_DIR=$ROOT_DIR/modules
HEADERS_DIR=$ROOT_DIR/headers
RDPARTY_DIR=$ROOT_DIR/sources/3rdparty

mkdir -p $MODULES_DIR
mkdir -p $HEADERS_DIR
mkdir -p $RDPARTY_DIR

git submodule init
git submodule update
git submodule foreach git pull

mkdir -p $ROOT_DIR/untests/catch
cp -fv $MODULES_DIR/catch2/single_include/catch2/catch.hpp $ROOT_DIR/untests/catch/catch.hpp

mkdir -p $RDPARTY_DIR/stb
cp -fv $MODULES_DIR/stb/stb_image.h $RDPARTY_DIR/stb/stb_image.h
cp -fv $MODULES_DIR/stb/stb_image_resize.h $RDPARTY_DIR/stb/stb_image_resize.h
cp -fv $MODULES_DIR/stb/stb_image_write.h $RDPARTY_DIR/stb/stb_image_write.h
cp -fv $MODULES_DIR/stb/stb_rect_pack.h $RDPARTY_DIR/stb/stb_rect_pack.h
cp -fv $MODULES_DIR/stb/stb_sprintf.h $RDPARTY_DIR/stb/stb_sprintf.h
cp -fv $MODULES_DIR/stb/stb_truetype.h $RDPARTY_DIR/stb/stb_truetype.h

mkdir -p $RDPARTY_DIR/utfcpp
cp -rfv $MODULES_DIR/utfcpp/source/ $RDPARTY_DIR/utfcpp/
