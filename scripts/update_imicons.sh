#!/bin/bash
ROOT_DIR=`dirname "$BASH_SOURCE"`/..
MODULES_DIR=$ROOT_DIR/modules
HEADERS_DIR=$ROOT_DIR/headers
SOURCES_DIR=$ROOT_DIR/sources
HEADERS_RDPARTY_DIR=$HEADERS_DIR/3rdparty
SOURCES_RDPARTY_DIR=$SOURCES_DIR/3rdparty

mkdir -p $MODULES_DIR
mkdir -p $HEADERS_DIR
mkdir -p $SOURCES_DIR
mkdir -p $HEADERS_RDPARTY_DIR
mkdir -p $SOURCES_RDPARTY_DIR

HEADERS_IMICONS_DIR=$HEADERS_RDPARTY_DIR/imicons
SOURCES_IMICONS_DIR=$SOURCES_RDPARTY_DIR/imicons
SOURCES_IMICONS_BUILD_DIR=$SOURCES_IMICONS_DIR/build

mkdir -p $HEADERS_IMICONS_DIR
mkdir -p $SOURCES_IMICONS_DIR
mkdir -p $SOURCES_IMICONS_BUILD_DIR

cp -fv $MODULES_DIR/imgui/misc/fonts/binary_to_compressed_c.cpp $SOURCES_IMICONS_BUILD_DIR/binary_to_compressed_c.cpp

pushd $SOURCES_IMICONS_BUILD_DIR
wget -q --show-progress -N https://github.com/FortAwesome/Font-Awesome/raw/master/webfonts/fa-solid-900.ttf
wget -q --show-progress -N https://github.com/FortAwesome/Font-Awesome/raw/master/webfonts/fa-regular-400.ttf
wget -q --show-progress -N https://github.com/google/material-design-icons/raw/master/font/MaterialIcons-Regular.ttf
wget -q --show-progress -N https://raw.githubusercontent.com/juliettef/IconFontCppHeaders/master/IconsFontAwesome5.h
wget -q --show-progress -N https://raw.githubusercontent.com/juliettef/IconFontCppHeaders/master/IconsMaterialDesign.h

clang++ binary_to_compressed_c.cpp -o binary_to_compressed_c
./binary_to_compressed_c fa-solid-900.ttf fa_solid > fa_solid.bin.h
./binary_to_compressed_c fa-regular-400.ttf fa_regular > fa_regular.bin.h
./binary_to_compressed_c MaterialIcons-Regular.ttf md_regular > md_regular.bin.h
popd

cp -fv $SOURCES_IMICONS_BUILD_DIR/fa_solid.bin.h $SOURCES_IMICONS_DIR
cp -fv $SOURCES_IMICONS_BUILD_DIR/fa_regular.bin.h $SOURCES_IMICONS_DIR
cp -fv $SOURCES_IMICONS_BUILD_DIR/md_regular.bin.h $SOURCES_IMICONS_DIR

cp -fv $SOURCES_IMICONS_BUILD_DIR/IconsFontAwesome5.h $HEADERS_IMICONS_DIR/icons_fa.h
cp -fv $SOURCES_IMICONS_BUILD_DIR/IconsMaterialDesign.h $HEADERS_IMICONS_DIR/icons_md.h

rm -rf $SOURCES_IMICONS_BUILD_DIR
