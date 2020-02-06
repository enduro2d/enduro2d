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

HEADERS_ICONS_DIR=$HEADERS_RDPARTY_DIR/icons
SOURCES_ICONS_DIR=$SOURCES_RDPARTY_DIR/icons
SOURCES_ICONS_BUILD_DIR=$SOURCES_RDPARTY_DIR/icons/build

mkdir -p $HEADERS_ICONS_DIR
cp -fv $MODULES_DIR/IconFontCppHeaders/IconsFontAwesome5.h $HEADERS_ICONS_DIR/icon_fa.h
cp -fv $MODULES_DIR/IconFontCppHeaders/IconsMaterialDesign.h $HEADERS_ICONS_DIR/icon_md.h

mkdir -p $SOURCES_ICONS_BUILD_DIR
cp -fv $MODULES_DIR/imgui/misc/fonts/binary_to_compressed_c.cpp $SOURCES_ICONS_BUILD_DIR/binary_to_compressed_c.cpp
wget -N -O $SOURCES_ICONS_BUILD_DIR/fa_solid_900.ttf https://github.com/FortAwesome/Font-Awesome/raw/master/webfonts/fa-solid-900.ttf
wget -N -O $SOURCES_ICONS_BUILD_DIR/fa_regular_400.ttf https://github.com/FortAwesome/Font-Awesome/raw/master/webfonts/fa-regular-400.ttf
wget -N -O $SOURCES_ICONS_BUILD_DIR/materialicons_regular.ttf https://github.com/google/material-design-icons/raw/master/iconfont/MaterialIcons-Regular.ttf

pushd $SOURCES_ICONS_BUILD_DIR
clang++ binary_to_compressed_c.cpp -o binary_to_compressed_c
./binary_to_compressed_c fa_solid_900.ttf fa_solid_900 > ../fa_solid_900.h
./binary_to_compressed_c fa_regular_400.ttf fa_regular_400 > ../fa_regular_400.h
./binary_to_compressed_c materialicons_regular.ttf materialicons_regular > ../materialicons_regular.h
popd

rm -rf $SOURCES_ICONS_BUILD_DIR
