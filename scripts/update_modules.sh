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

#
# catch
#

mkdir -p $UNTESTS_DIR/catch
cp -fv $MODULES_DIR/catch2/single_include/catch2/catch.hpp $UNTESTS_DIR/catch/catch.hpp

#
# imgui
#

mkdir -p $HEADERS_RDPARTY_DIR/imgui
cp -fv $MODULES_DIR/imgui/imgui_internal.h $HEADERS_RDPARTY_DIR/imgui/imgui_internal.h
cp -fv $MODULES_DIR/imgui/misc/cpp/imgui_stdlib.h $HEADERS_RDPARTY_DIR/imgui/imgui_stdlib.h
cp -fv $MODULES_DIR/imgui/imgui.h $HEADERS_RDPARTY_DIR/imgui/imgui.h
cp -fv $MODULES_DIR/imgui/imstb_rectpack.h $HEADERS_RDPARTY_DIR/imgui/imstb_rectpack.h
cp -fv $MODULES_DIR/imgui/imstb_textedit.h $HEADERS_RDPARTY_DIR/imgui/imstb_textedit.h
cp -fv $MODULES_DIR/imgui/imstb_truetype.h $HEADERS_RDPARTY_DIR/imgui/imstb_truetype.h

mkdir -p $SOURCES_RDPARTY_DIR/imgui
cp -fv $MODULES_DIR/imgui/imgui.cpp $SOURCES_RDPARTY_DIR/imgui/imgui.cpp
cp -fv $MODULES_DIR/imgui/imgui_demo.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_demo.cpp
cp -fv $MODULES_DIR/imgui/imgui_draw.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_draw.cpp
cp -fv $MODULES_DIR/imgui/imgui_widgets.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_widgets.cpp
cp -fv $MODULES_DIR/imgui/misc/cpp/imgui_stdlib.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_stdlib.cpp

#
# lua
#

mkdir -p $HEADERS_RDPARTY_DIR/lua
cp -fv $MODULES_DIR/lua/lauxlib.h $HEADERS_RDPARTY_DIR/lua/lauxlib.h
cp -fv $MODULES_DIR/lua/lua.h $HEADERS_RDPARTY_DIR/lua/lua.h
cp -fv $MODULES_DIR/lua/luaconf.h $HEADERS_RDPARTY_DIR/lua/luaconf.h
cp -fv $MODULES_DIR/lua/lualib.h $HEADERS_RDPARTY_DIR/lua/lualib.h

mkdir -p $SOURCES_RDPARTY_DIR/lua
cp -fv $MODULES_DIR/lua/lapi.c $SOURCES_RDPARTY_DIR/lua/lapi.c
cp -fv $MODULES_DIR/lua/lapi.h $SOURCES_RDPARTY_DIR/lua/lapi.h
cp -fv $MODULES_DIR/lua/lauxlib.c $SOURCES_RDPARTY_DIR/lua/lauxlib.c
cp -fv $MODULES_DIR/lua/lbaselib.c $SOURCES_RDPARTY_DIR/lua/lbaselib.c
cp -fv $MODULES_DIR/lua/lbitlib.c $SOURCES_RDPARTY_DIR/lua/lbitlib.c
cp -fv $MODULES_DIR/lua/lcode.c $SOURCES_RDPARTY_DIR/lua/lcode.c
cp -fv $MODULES_DIR/lua/lcode.h $SOURCES_RDPARTY_DIR/lua/lcode.h
cp -fv $MODULES_DIR/lua/lcorolib.c $SOURCES_RDPARTY_DIR/lua/lcorolib.c
cp -fv $MODULES_DIR/lua/lctype.c $SOURCES_RDPARTY_DIR/lua/lctype.c
cp -fv $MODULES_DIR/lua/lctype.h $SOURCES_RDPARTY_DIR/lua/lctype.h
cp -fv $MODULES_DIR/lua/ldblib.c $SOURCES_RDPARTY_DIR/lua/ldblib.c
cp -fv $MODULES_DIR/lua/ldebug.c $SOURCES_RDPARTY_DIR/lua/ldebug.c
cp -fv $MODULES_DIR/lua/ldebug.h $SOURCES_RDPARTY_DIR/lua/ldebug.h
cp -fv $MODULES_DIR/lua/ldo.c $SOURCES_RDPARTY_DIR/lua/ldo.c
cp -fv $MODULES_DIR/lua/ldo.h $SOURCES_RDPARTY_DIR/lua/ldo.h
cp -fv $MODULES_DIR/lua/ldump.c $SOURCES_RDPARTY_DIR/lua/ldump.c
cp -fv $MODULES_DIR/lua/lfunc.c $SOURCES_RDPARTY_DIR/lua/lfunc.c
cp -fv $MODULES_DIR/lua/lfunc.h $SOURCES_RDPARTY_DIR/lua/lfunc.h
cp -fv $MODULES_DIR/lua/lgc.c $SOURCES_RDPARTY_DIR/lua/lgc.c
cp -fv $MODULES_DIR/lua/lgc.h $SOURCES_RDPARTY_DIR/lua/lgc.h
cp -fv $MODULES_DIR/lua/linit.c $SOURCES_RDPARTY_DIR/lua/linit.c
cp -fv $MODULES_DIR/lua/liolib.c $SOURCES_RDPARTY_DIR/lua/liolib.c
cp -fv $MODULES_DIR/lua/llex.c $SOURCES_RDPARTY_DIR/lua/llex.c
cp -fv $MODULES_DIR/lua/llex.h $SOURCES_RDPARTY_DIR/lua/llex.h
cp -fv $MODULES_DIR/lua/llimits.h $SOURCES_RDPARTY_DIR/lua/llimits.h
cp -fv $MODULES_DIR/lua/lmathlib.c $SOURCES_RDPARTY_DIR/lua/lmathlib.c
cp -fv $MODULES_DIR/lua/lmem.c $SOURCES_RDPARTY_DIR/lua/lmem.c
cp -fv $MODULES_DIR/lua/lmem.h $SOURCES_RDPARTY_DIR/lua/lmem.h
cp -fv $MODULES_DIR/lua/loadlib.c $SOURCES_RDPARTY_DIR/lua/loadlib.c
cp -fv $MODULES_DIR/lua/lobject.c $SOURCES_RDPARTY_DIR/lua/lobject.c
cp -fv $MODULES_DIR/lua/lobject.h $SOURCES_RDPARTY_DIR/lua/lobject.h
cp -fv $MODULES_DIR/lua/lopcodes.c $SOURCES_RDPARTY_DIR/lua/lopcodes.c
cp -fv $MODULES_DIR/lua/lopcodes.h $SOURCES_RDPARTY_DIR/lua/lopcodes.h
cp -fv $MODULES_DIR/lua/loslib.c $SOURCES_RDPARTY_DIR/lua/loslib.c
cp -fv $MODULES_DIR/lua/lparser.c $SOURCES_RDPARTY_DIR/lua/lparser.c
cp -fv $MODULES_DIR/lua/lparser.h $SOURCES_RDPARTY_DIR/lua/lparser.h
cp -fv $MODULES_DIR/lua/lprefix.h $SOURCES_RDPARTY_DIR/lua/lprefix.h
cp -fv $MODULES_DIR/lua/lstate.c $SOURCES_RDPARTY_DIR/lua/lstate.c
cp -fv $MODULES_DIR/lua/lstate.h $SOURCES_RDPARTY_DIR/lua/lstate.h
cp -fv $MODULES_DIR/lua/lstring.c $SOURCES_RDPARTY_DIR/lua/lstring.c
cp -fv $MODULES_DIR/lua/lstring.h $SOURCES_RDPARTY_DIR/lua/lstring.h
cp -fv $MODULES_DIR/lua/lstrlib.c $SOURCES_RDPARTY_DIR/lua/lstrlib.c
cp -fv $MODULES_DIR/lua/ltable.c $SOURCES_RDPARTY_DIR/lua/ltable.c
cp -fv $MODULES_DIR/lua/ltable.h $SOURCES_RDPARTY_DIR/lua/ltable.h
cp -fv $MODULES_DIR/lua/ltablib.c $SOURCES_RDPARTY_DIR/lua/ltablib.c
cp -fv $MODULES_DIR/lua/ltm.c $SOURCES_RDPARTY_DIR/lua/ltm.c
cp -fv $MODULES_DIR/lua/ltm.h $SOURCES_RDPARTY_DIR/lua/ltm.h
cp -fv $MODULES_DIR/lua/lundump.c $SOURCES_RDPARTY_DIR/lua/lundump.c
cp -fv $MODULES_DIR/lua/lundump.h $SOURCES_RDPARTY_DIR/lua/lundump.h
cp -fv $MODULES_DIR/lua/lutf8lib.c $SOURCES_RDPARTY_DIR/lua/lutf8lib.c
cp -fv $MODULES_DIR/lua/lvm.c $SOURCES_RDPARTY_DIR/lua/lvm.c
cp -fv $MODULES_DIR/lua/lvm.h $SOURCES_RDPARTY_DIR/lua/lvm.h
cp -fv $MODULES_DIR/lua/lzio.c $SOURCES_RDPARTY_DIR/lua/lzio.c
cp -fv $MODULES_DIR/lua/lzio.h $SOURCES_RDPARTY_DIR/lua/lzio.h

#
# miniz
#

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

#
# pnpoly
#

mkdir -p $SOURCES_RDPARTY_DIR/pnpoly.h
cp -fv $MODULES_DIR/pnpoly.h/pnpoly.h $SOURCES_RDPARTY_DIR/pnpoly.h/pnpoly.h

#
# pugixml
#

mkdir -p $HEADERS_RDPARTY_DIR/pugixml
cp -fv $MODULES_DIR/pugixml/src/pugiconfig.hpp $HEADERS_RDPARTY_DIR/pugixml/pugiconfig.hpp
cp -fv $MODULES_DIR/pugixml/src/pugixml.hpp $HEADERS_RDPARTY_DIR/pugixml/pugixml.hpp

mkdir -p $SOURCES_RDPARTY_DIR/pugixml
cp -fv $MODULES_DIR/pugixml/src/pugixml.cpp $SOURCES_RDPARTY_DIR/pugixml/pugixml.cpp

#
# rapidjson
#

mkdir -p $HEADERS_RDPARTY_DIR/rapidjson
cp -rfv $MODULES_DIR/rapidjson/include/rapidjson/. $HEADERS_RDPARTY_DIR/rapidjson

#
# sol
#

mkdir -p $HEADERS_RDPARTY_DIR/sol
cp -rfv $MODULES_DIR/sol2/include/sol/. $HEADERS_RDPARTY_DIR/sol

#
# stb
#

mkdir -p $SOURCES_RDPARTY_DIR/stb
cp -fv $MODULES_DIR/stb/stb_image.h $SOURCES_RDPARTY_DIR/stb/stb_image.h
cp -fv $MODULES_DIR/stb/stb_image_resize.h $SOURCES_RDPARTY_DIR/stb/stb_image_resize.h
cp -fv $MODULES_DIR/stb/stb_image_write.h $SOURCES_RDPARTY_DIR/stb/stb_image_write.h
cp -fv $MODULES_DIR/stb/stb_rect_pack.h $SOURCES_RDPARTY_DIR/stb/stb_rect_pack.h
cp -fv $MODULES_DIR/stb/stb_sprintf.h $SOURCES_RDPARTY_DIR/stb/stb_sprintf.h
cp -fv $MODULES_DIR/stb/stb_truetype.h $SOURCES_RDPARTY_DIR/stb/stb_truetype.h

#
# utfcpp
#

mkdir -p $SOURCES_RDPARTY_DIR/utfcpp
cp -rfv $MODULES_DIR/utfcpp/source/. $SOURCES_RDPARTY_DIR/utfcpp/

#
# yoga
#

mkdir -p $SOURCES_RDPARTY_DIR/yoga
cp -rfv $MODULES_DIR/yoga/yoga/. $SOURCES_RDPARTY_DIR/yoga/
