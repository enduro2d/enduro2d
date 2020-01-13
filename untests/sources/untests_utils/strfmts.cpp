/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("strfmts") {
    {
        char buf[2];
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", v2i::zero()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", v3i::zero()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", v4i::zero()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", m2i::identity()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", m3i::identity()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", m4i::identity()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_rad(10)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_deg(10)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", v2f::zero()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", v3f::zero()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", v4f::zero()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", m2f::identity()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", m3f::identity()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", m4f::identity()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_rad(10.f)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_deg(10.0)), strings::bad_format_buffer);
    }
    {
        char buf[2];
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", color::white()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", color32::white()), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_seconds(0)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_milliseconds(0)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_microseconds(0)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_seconds(0.f)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_milliseconds(0.f)), strings::bad_format_buffer);
        REQUIRE_THROWS_AS(
            strings::format(buf, sizeof(buf), "%0", make_microseconds(0.f)), strings::bad_format_buffer);
    }
    {
        REQUIRE(strings::rformat(
            "%0",
            v2u(1,2)) == "(1,2)");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(v3u(1,2,3), u8(3))) == "(  1,  2,  3)");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(v4i(1,2,3,4), u8(2))) == "( 1, 2, 3, 4)");

        REQUIRE(strings::rformat(
            "%0",
            v2f(1.f,2.f)) == "(1.000000,2.000000)");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(v3d(1,2,3), u8(5), u8(2))) == "( 1.00, 2.00, 3.00)");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(v4f(1,2,3,4), u8(0), u8(1))) == "(1.0,2.0,3.0,4.0)");
    }
    {
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(m2i(1,2,3,4), u8(3))) ==
            "((  1,  2),(  3,  4))");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(m3i(1,2,3,4,5,6,7,8,9), u8(1))) ==
            "((1,2,3),(4,5,6),(7,8,9))");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(m4i(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), u8(2))) ==
            "(( 1, 2, 3, 4),( 5, 6, 7, 8),( 9,10,11,12),(13,14,15,16))");

        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(m2f(1,2,3,4), u8(5), u8(2))) ==
            "(( 1.00, 2.00),( 3.00, 4.00))");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(m3f(1,2,3,4,5,6,7,8,9), u8(4), u8(1))) ==
            "(( 1.0, 2.0, 3.0),( 4.0, 5.0, 6.0),( 7.0, 8.0, 9.0))");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(m4f(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), u8(2), u8(0))) ==
            "(( 1, 2, 3, 4),( 5, 6, 7, 8),( 9,10,11,12),(13,14,15,16))");
    }
    {
        REQUIRE(strings::rformat("%0", make_rad(2)) == "2rad");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(make_rad(2.f), u8(5), u8(2))) ==
            " 2.00rad");

        REQUIRE(strings::rformat("%0", make_deg(3)) == "3deg");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(make_deg(2.0), u8(5), u8(2))) ==
            " 2.00deg");
    }
    {
        str utf8 = make_utf8("hello");
        wstr wide = make_wide("hello");
        str16 utf16 = make_utf16("hello");
        str32 utf32 = make_utf32("hello");

        REQUIRE(strings::rformat("%0", utf8) == utf8);
        REQUIRE(strings::rformat("%0", wide) == utf8);
        REQUIRE(strings::rformat("%0", utf16) == utf8);
        REQUIRE(strings::rformat("%0", utf32) == utf8);
    }
    {
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(color(1.f,2.f,3.f,4.f), u8(0), u8(2))) ==
            "(1.00,2.00,3.00,4.00)");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(color32(1,2,3,4), u8(2))) == "( 1, 2, 3, 4)");
    }
    {
        REQUIRE(strings::rformat("%0", make_seconds(2)) == "2s");
        REQUIRE(strings::rformat("%0", make_milliseconds(3)) == "3ms");
        REQUIRE(strings::rformat("%0", make_microseconds(4)) == "4us");

        REQUIRE(strings::rformat("%0", make_seconds(2.f)) == "2.000000s");
        REQUIRE(strings::rformat("%0", make_milliseconds(3.0)) == "3.000000ms");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(make_microseconds(4.f), u8(5), u8(2))) ==
            " 4.00us");
    }
    {
        REQUIRE(strings::rformat("%0", make_rect(1,2,3,4)) == "(1,2,3,4)");
        REQUIRE(strings::rformat("%0", make_aabb(1,2,3,4,5,6)) == "(1,2,3,4,5,6)");

        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(make_rect(1.f,2.f,3.f,4.f), u8(5), u8(2))) == "( 1.00, 2.00, 3.00, 4.00)");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(make_aabb(1.f,2.f,3.f,4.f,5.f,6.f), u8(5), u8(2))) == "( 1.00, 2.00, 3.00, 4.00, 5.00, 6.00)");
    }
    {
        REQUIRE(strings::rformat("%0", image_file_format::tga) == "tga");
        REQUIRE(strings::rformat(
            "%0",
            strings::make_format_arg(image_file_format::dds, u8(5))) ==
            "  dds");
    }
}
