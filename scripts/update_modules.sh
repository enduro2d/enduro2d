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

mkdir -p $UNTESTS_DIR/catch
cp -fv $MODULES_DIR/catch2/single_include/catch2/catch.hpp $UNTESTS_DIR/catch/catch.hpp

mkdir -p $HEADERS_RDPARTY_DIR/imgui
cp -fv $MODULES_DIR/imgui/imgui.cpp $HEADERS_RDPARTY_DIR/imgui/imgui.cpp
cp -fv $MODULES_DIR/imgui/imgui.h $HEADERS_RDPARTY_DIR/imgui/imgui.h
cp -fv $MODULES_DIR/imgui/imgui_demo.cpp $HEADERS_RDPARTY_DIR/imgui/imgui_demo.cpp
cp -fv $MODULES_DIR/imgui/imgui_draw.cpp $HEADERS_RDPARTY_DIR/imgui/imgui_draw.cpp
cp -fv $MODULES_DIR/imgui/imgui_internal.h $HEADERS_RDPARTY_DIR/imgui/imgui_internal.h
cp -fv $MODULES_DIR/imgui/imgui_widgets.cpp $HEADERS_RDPARTY_DIR/imgui/imgui_widgets.cpp
cp -fv $MODULES_DIR/imgui/imstb_rectpack.h $HEADERS_RDPARTY_DIR/imgui/imstb_rectpack.h
cp -fv $MODULES_DIR/imgui/imstb_textedit.h $HEADERS_RDPARTY_DIR/imgui/imstb_textedit.h
cp -fv $MODULES_DIR/imgui/imstb_truetype.h $HEADERS_RDPARTY_DIR/imgui/imstb_truetype.h
cp -fv $MODULES_DIR/imgui/misc/cpp/imgui_stdlib.h $HEADERS_RDPARTY_DIR/imgui/imgui_stdlib.h
cp -fv $MODULES_DIR/imgui/misc/cpp/imgui_stdlib.cpp $HEADERS_RDPARTY_DIR/imgui/imgui_stdlib.cpp

mkdir -p $HEADERS_RDPARTY_DIR/lua
cp -fv $MODULES_DIR/lua/lapi.c $HEADERS_RDPARTY_DIR/lua/lapi.c
cp -fv $MODULES_DIR/lua/lapi.h $HEADERS_RDPARTY_DIR/lua/lapi.h
cp -fv $MODULES_DIR/lua/lauxlib.c $HEADERS_RDPARTY_DIR/lua/lauxlib.c
cp -fv $MODULES_DIR/lua/lauxlib.h $HEADERS_RDPARTY_DIR/lua/lauxlib.h
cp -fv $MODULES_DIR/lua/lbaselib.c $HEADERS_RDPARTY_DIR/lua/lbaselib.c
cp -fv $MODULES_DIR/lua/lbitlib.c $HEADERS_RDPARTY_DIR/lua/lbitlib.c
cp -fv $MODULES_DIR/lua/lcode.c $HEADERS_RDPARTY_DIR/lua/lcode.c
cp -fv $MODULES_DIR/lua/lcode.h $HEADERS_RDPARTY_DIR/lua/lcode.h
cp -fv $MODULES_DIR/lua/lcorolib.c $HEADERS_RDPARTY_DIR/lua/lcorolib.c
cp -fv $MODULES_DIR/lua/lctype.c $HEADERS_RDPARTY_DIR/lua/lctype.c
cp -fv $MODULES_DIR/lua/lctype.h $HEADERS_RDPARTY_DIR/lua/lctype.h
cp -fv $MODULES_DIR/lua/ldblib.c $HEADERS_RDPARTY_DIR/lua/ldblib.c
cp -fv $MODULES_DIR/lua/ldebug.c $HEADERS_RDPARTY_DIR/lua/ldebug.c
cp -fv $MODULES_DIR/lua/ldebug.h $HEADERS_RDPARTY_DIR/lua/ldebug.h
cp -fv $MODULES_DIR/lua/ldo.c $HEADERS_RDPARTY_DIR/lua/ldo.c
cp -fv $MODULES_DIR/lua/ldo.h $HEADERS_RDPARTY_DIR/lua/ldo.h
cp -fv $MODULES_DIR/lua/ldump.c $HEADERS_RDPARTY_DIR/lua/ldump.c
cp -fv $MODULES_DIR/lua/lfunc.c $HEADERS_RDPARTY_DIR/lua/lfunc.c
cp -fv $MODULES_DIR/lua/lfunc.h $HEADERS_RDPARTY_DIR/lua/lfunc.h
cp -fv $MODULES_DIR/lua/lgc.c $HEADERS_RDPARTY_DIR/lua/lgc.c
cp -fv $MODULES_DIR/lua/lgc.h $HEADERS_RDPARTY_DIR/lua/lgc.h
cp -fv $MODULES_DIR/lua/linit.c $HEADERS_RDPARTY_DIR/lua/linit.c
cp -fv $MODULES_DIR/lua/liolib.c $HEADERS_RDPARTY_DIR/lua/liolib.c
cp -fv $MODULES_DIR/lua/llex.c $HEADERS_RDPARTY_DIR/lua/llex.c
cp -fv $MODULES_DIR/lua/llex.h $HEADERS_RDPARTY_DIR/lua/llex.h
cp -fv $MODULES_DIR/lua/llimits.h $HEADERS_RDPARTY_DIR/lua/llimits.h
cp -fv $MODULES_DIR/lua/lmathlib.c $HEADERS_RDPARTY_DIR/lua/lmathlib.c
cp -fv $MODULES_DIR/lua/lmem.c $HEADERS_RDPARTY_DIR/lua/lmem.c
cp -fv $MODULES_DIR/lua/lmem.h $HEADERS_RDPARTY_DIR/lua/lmem.h
cp -fv $MODULES_DIR/lua/loadlib.c $HEADERS_RDPARTY_DIR/lua/loadlib.c
cp -fv $MODULES_DIR/lua/lobject.c $HEADERS_RDPARTY_DIR/lua/lobject.c
cp -fv $MODULES_DIR/lua/lobject.h $HEADERS_RDPARTY_DIR/lua/lobject.h
cp -fv $MODULES_DIR/lua/lopcodes.c $HEADERS_RDPARTY_DIR/lua/lopcodes.c
cp -fv $MODULES_DIR/lua/lopcodes.h $HEADERS_RDPARTY_DIR/lua/lopcodes.h
cp -fv $MODULES_DIR/lua/loslib.c $HEADERS_RDPARTY_DIR/lua/loslib.c
cp -fv $MODULES_DIR/lua/lparser.c $HEADERS_RDPARTY_DIR/lua/lparser.c
cp -fv $MODULES_DIR/lua/lparser.h $HEADERS_RDPARTY_DIR/lua/lparser.h
cp -fv $MODULES_DIR/lua/lprefix.h $HEADERS_RDPARTY_DIR/lua/lprefix.h
cp -fv $MODULES_DIR/lua/lstate.c $HEADERS_RDPARTY_DIR/lua/lstate.c
cp -fv $MODULES_DIR/lua/lstate.h $HEADERS_RDPARTY_DIR/lua/lstate.h
cp -fv $MODULES_DIR/lua/lstring.c $HEADERS_RDPARTY_DIR/lua/lstring.c
cp -fv $MODULES_DIR/lua/lstring.h $HEADERS_RDPARTY_DIR/lua/lstring.h
cp -fv $MODULES_DIR/lua/lstrlib.c $HEADERS_RDPARTY_DIR/lua/lstrlib.c
cp -fv $MODULES_DIR/lua/ltable.c $HEADERS_RDPARTY_DIR/lua/ltable.c
cp -fv $MODULES_DIR/lua/ltable.h $HEADERS_RDPARTY_DIR/lua/ltable.h
cp -fv $MODULES_DIR/lua/ltablib.c $HEADERS_RDPARTY_DIR/lua/ltablib.c
cp -fv $MODULES_DIR/lua/ltm.c $HEADERS_RDPARTY_DIR/lua/ltm.c
cp -fv $MODULES_DIR/lua/ltm.h $HEADERS_RDPARTY_DIR/lua/ltm.h
cp -fv $MODULES_DIR/lua/lua.h $HEADERS_RDPARTY_DIR/lua/lua.h
cp -fv $MODULES_DIR/lua/luaconf.h $HEADERS_RDPARTY_DIR/lua/luaconf.h
cp -fv $MODULES_DIR/lua/lualib.h $HEADERS_RDPARTY_DIR/lua/lualib.h
cp -fv $MODULES_DIR/lua/lundump.c $HEADERS_RDPARTY_DIR/lua/lundump.c
cp -fv $MODULES_DIR/lua/lundump.h $HEADERS_RDPARTY_DIR/lua/lundump.h
cp -fv $MODULES_DIR/lua/lutf8lib.c $HEADERS_RDPARTY_DIR/lua/lutf8lib.c
cp -fv $MODULES_DIR/lua/lvm.c $HEADERS_RDPARTY_DIR/lua/lvm.c
cp -fv $MODULES_DIR/lua/lvm.h $HEADERS_RDPARTY_DIR/lua/lvm.h
cp -fv $MODULES_DIR/lua/lzio.c $HEADERS_RDPARTY_DIR/lua/lzio.c
cp -fv $MODULES_DIR/lua/lzio.h $HEADERS_RDPARTY_DIR/lua/lzio.h

mkdir -p $SOURCES_RDPARTY_DIR/miniz
cp -fv $MODULES_DIR/miniz/miniz.c $SOURCES_RDPARTY_DIR/miniz/miniz.c
cp -fv $MODULES_DIR/miniz/miniz.h $SOURCES_RDPARTY_DIR/miniz/miniz.h
cp -fv $MODULES_DIR/miniz/miniz_common.h $SOURCES_RDPARTY_DIR/miniz/miniz_common.h
cp -fv $MODULES_DIR/miniz/miniz_tdef.c $SOURCES_RDPARTY_DIR/miniz/miniz_tdef.c
cp -fv $MODULES_DIR/miniz/miniz_tdef.h $SOURCES_RDPARTY_DIR/miniz/miniz_tdef.h
cp -fv $MODULES_DIR/miniz/miniz_tinfl.c $SOURCES_RDPARTY_DIR/miniz/miniz_tinfl.c
cp -fv $MODULES_DIR/miniz/miniz_tinfl.h $SOURCES_RDPARTY_DIR/miniz/miniz_tinfl.h
cp -fv $MODULES_DIR/miniz/miniz_zip.c $SOURCES_RDPARTY_DIR/miniz/miniz_zip.c
cp -fv $MODULES_DIR/miniz/miniz_zip.h $SOURCES_RDPARTY_DIR/miniz/miniz_zip.h

mkdir -p $SOURCES_RDPARTY_DIR/pnpoly.h
cp -fv $MODULES_DIR/pnpoly.h/pnpoly.h $SOURCES_RDPARTY_DIR/pnpoly.h/pnpoly.h

mkdir -p $HEADERS_RDPARTY_DIR/pugixml
cp -rfv $MODULES_DIR/pugixml/src/. $HEADERS_RDPARTY_DIR/pugixml/

mkdir -p $HEADERS_RDPARTY_DIR/rapidjson
cp -rfv $MODULES_DIR/rapidjson/include/rapidjson/. $HEADERS_RDPARTY_DIR/rapidjson

mkdir -p $HEADERS_RDPARTY_DIR/sol
cp -rfv $MODULES_DIR/sol2/include/sol/. $HEADERS_RDPARTY_DIR/sol

mkdir -p $SOURCES_RDPARTY_DIR/stb
cp -fv $MODULES_DIR/stb/stb_image.h $SOURCES_RDPARTY_DIR/stb/stb_image.h
cp -fv $MODULES_DIR/stb/stb_image_resize.h $SOURCES_RDPARTY_DIR/stb/stb_image_resize.h
cp -fv $MODULES_DIR/stb/stb_image_write.h $SOURCES_RDPARTY_DIR/stb/stb_image_write.h
cp -fv $MODULES_DIR/stb/stb_rect_pack.h $SOURCES_RDPARTY_DIR/stb/stb_rect_pack.h
cp -fv $MODULES_DIR/stb/stb_sprintf.h $SOURCES_RDPARTY_DIR/stb/stb_sprintf.h
cp -fv $MODULES_DIR/stb/stb_truetype.h $SOURCES_RDPARTY_DIR/stb/stb_truetype.h

mkdir -p $SOURCES_RDPARTY_DIR/utfcpp
cp -rfv $MODULES_DIR/utfcpp/source/. $SOURCES_RDPARTY_DIR/utfcpp/
