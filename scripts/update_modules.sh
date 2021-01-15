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
# glew
#

mkdir -p $SOURCES_RDPARTY_DIR/glew
cp -rfv $MODULES_DIR/glew/include/. $SOURCES_RDPARTY_DIR/glew/
cp -fv $MODULES_DIR/glew/src/glew.c $SOURCES_RDPARTY_DIR/glew/glew.c

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
cp -fv $MODULES_DIR/imgui/imgui_tables.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_tables.cpp
cp -fv $MODULES_DIR/imgui/imgui_widgets.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_widgets.cpp
cp -fv $MODULES_DIR/imgui/misc/cpp/imgui_stdlib.cpp $SOURCES_RDPARTY_DIR/imgui/imgui_stdlib.cpp

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
