/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

namespace
{
    const char* const mini_fnt = R"fnt(
        info face ="Arial" size=  50 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=0,0
        common  lineHeight  =58   base=  49  scaleW  = 512 scaleH=512 pages=1 packed=0
        page id=0 file="arial.png"

        chars count=3
        char id=32 x=342 y=38 width=0 height=0 xoffset=0 yoffset=46 xadvance=14 page=0 chnl=0
        char id=33 x=448 y=386 width=10 height=40 xoffset=5 yoffset=10 xadvance=14 page=0 chnl=0
        char id=34 x=394 y=306 width=18 height=18 xoffset=3 yoffset=8 xadvance=18 page=0 chnl=0

        kernings count=2
        kerning first=1059 second=1081 amount=-1
        kerning first=1043 second=1071 amount=-1
    )fnt";

    const char* const mini_bad_fnt = R"fnt(
        info face="Arial" size=HELLO bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=0,0
    )fnt";
}

TEST_CASE("font") {
    {
        font f;
        REQUIRE_FALSE(fonts::try_load_font(f, buffer(mini_bad_fnt, std::strlen(mini_bad_fnt))));
        REQUIRE_FALSE(fonts::try_load_font(f, make_memory_stream(buffer(mini_bad_fnt, std::strlen(mini_bad_fnt)))));
    }
    {
        font f, f2;
        REQUIRE(fonts::try_load_font(f, buffer(mini_fnt, std::strlen(mini_fnt))));
        REQUIRE(fonts::try_load_font(f2, make_memory_stream(buffer(mini_fnt, std::strlen(mini_fnt)))));
        REQUIRE(f == f2);

        REQUIRE(f.info().atlas_file == "arial.png");
        REQUIRE(f.info().atlas_size == v2u(512,512));
        REQUIRE(f.info().font_size == 50);
        REQUIRE(f.info().line_height == 58);
        REQUIRE(f.info().glyph_ascent == 49);

        REQUIRE(f.glyphs().size() == 3);
        REQUIRE_FALSE(f.find_glyph(100500));
        REQUIRE(f.find_glyph(33));
        {
            // char id=33 x=448 y=386 width=10 height=40 xoffset=5 yoffset=10 xadvance=14 page=0 chnl=0
            font::glyph_info c;
            c.offset = v2i(5, -50);
            c.tex_rect = b2u(448, 86, 10, 40);
            c.advance = 14;
            REQUIRE(*f.find_glyph(33) == c);
        }

        REQUIRE(f.kernings().size() == 2);
        REQUIRE(f.get_kerning(1059, 1081) == -1);
        REQUIRE(f.get_kerning(1059, 100500) == 0);
    }
}
