/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

namespace
{
    const char* const mini_fnt = R"fnt(
        info face="Arial" size=50 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=0,0
        common lineHeight=58 base=49 scaleW=512 scaleH=512 pages=1 packed=0
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

        REQUIRE(f.info().face == "Arial");
        REQUIRE(f.info().size == 50);

        REQUIRE(f.common().line == 58);
        REQUIRE(f.common().base == 49);
        REQUIRE(f.common().pages == 1);
        REQUIRE(f.common().atlas_size == v2u(512,512));

        REQUIRE(f.pages().size() == 1);
        REQUIRE(f.find_page(0));
        REQUIRE(f.find_page(0)->id == 0);
        REQUIRE(f.find_page(0)->file == "arial.png");

        REQUIRE(f.chars().size() == 3);
        REQUIRE_FALSE(f.find_char(100500));
        REQUIRE(f.find_char(33));
        {
            // char id=33 x=448 y=386 width=10 height=40 xoffset=5 yoffset=10 xadvance=14 page=0 chnl=0
            font::char_data c;
            c.id = 33;
            c.rect = b2hi(448, 86, 10, 40);
            c.offset = v2hi(5, 10);
            c.advance = 14;
            c.page = 0;
            c.chnl = 0;
            REQUIRE(*f.find_char(33) == c);
        }

        REQUIRE(f.kernings().size() == 2);
        REQUIRE(f.get_kerning(1059, 1081) == -1);
        REQUIRE(f.get_kerning(1059, 100500) == 0);
    }
}
