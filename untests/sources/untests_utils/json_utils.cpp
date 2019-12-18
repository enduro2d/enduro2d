/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

namespace
{
    const char* json_source = R"json({
        "v0" : 42,
        "v1" : -42,

        "v2" : { "x" : 1 },
        "v3" : { "x" : 1, "y" : 2 },
        "v4" : { "x" : 1, "y" : 2, "z" : 3 },
        "v5" : { "x" : 1, "y" : 2, "z" : 3, "w" : 4 },
        "v6" : { "x" : 1, "y" : -2, "z" : 3, "w" : -4 },

        "v2_" : [1],
        "v3_" : [1,2],
        "v4_" : [1,2,3],
        "v5_" : [1,2,3,4],
        "v6_" : [1,-2,3,-4],

        "v0f" : 42.2,
        "v5f" : { "x" : 1.1, "y" : 2.2, "z" : 3.3, "w" : 4.4 },

        "m22" : [[1,2],[3,4]],
        "m33" : [[1,2,3],[3,4,5],[5,6,7]],
        "m44" : [[1,2,3,4],[3,4,5,6],[5,6,7,8],[7,8,9,0]],

        "m22_" : [1,2,3,4],
        "m33_" : [1,2,3,3,4,5,5,6,7],
        "m44_" : [1,2,3,4,3,4,5,6,5,6,7,8,7,8,9,0],

        "b2_0" : { "x" : 1, "w" : 2 },
        "b2_1" : { "x" : 1, "y" : 2, "w" : 3, "h" : 4 },
        "b2_2" : { "x" : 1, "y" : -2, "w" : 3, "h" : 4 },
        "b2_3" : [1,2],
        "b2_4" : [1,2,3,4],
        "b2_5" : [1,2,3],

        "b3_0" : { "x" : 1, "w" : 2 },
        "b3_1" : { "x" : 1, "y" : 2, "z" : 3, "w" : 4, "h" : 5, "d" : 6},
        "b3_2" : { "x" : 1, "y" : 2, "z" : 3, "w" : -4, "h" : 5, "d" : 6},
        "b3_3" : [1,2,3],
        "b3_4" : [1,2,3,4,5,6],
        "b3_5" : [1,2,3,4,5,6,7],

        "t2" : {
            "translation" : [1,2],
            "rotation" : 3,
            "scale" : [4,5]
        },

        "t3" : {
            "translation" : [1,2,3],
            "rotation" : [4,5,6],
            "scale" : [8,9,10]
        },

        "c0" : 0.5,
        "c1" : { "r" : 0.1, "b" : 0.2 },
        "c2" : { "r" : 0.1, "g" : 0.2, "b" : 0.3, "a" : 0.4 },
        "c0_" : "hello",
        "c1_" : { "r" : 0.5, "b" : {} },

        "c32_0" : 20,
        "c32_1" : { "r" : 10, "b" : 20 },
        "c32_2" : { "r" : 10, "g" : 20, "b" : 30, "a" : 40 },
        "c32_0_" : 1.2,
        "c32_1_" : { "r" : 20, "b" : {} },

        "s0" : "hello",
        "s1" : 10,
        "s2" : {},

        "tt" : true,
        "ff" : false,
        "tt_" : 1,
        "ff_" : [],

        "i" : 42,
        "i_" : "4",
        "u" : 42,
        "u_" : -42,
        "f0" : 1.2,
        "f1" : 2,
        "f_" : "hi",

        "vv" : [1,2,2,3],
        "vv_" : "hello",

        "e0" : "pvr",
        "e0_" : "hello",
        "e1" : "rgb_etc1",
        "e1_" : "hello"
    })json";
}

TEST_CASE("json_utils") {
    rapidjson::Document doc;
    REQUIRE_FALSE(doc.Parse(json_source).HasParseError());
    REQUIRE(doc.IsObject());
    {
        bool t = false;
        REQUIRE(json_utils::try_parse_value(doc["tt"], t));
        REQUIRE(t);

        bool f = true;
        REQUIRE(json_utils::try_parse_value(doc["ff"], f));
        REQUIRE_FALSE(f);

        REQUIRE_FALSE(json_utils::try_parse_value(doc["tt_"], t));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["ff_"], f));

        int i = 0;
        REQUIRE(json_utils::try_parse_value(doc["i"], i));
        REQUIRE(i == 42);
        REQUIRE_FALSE(json_utils::try_parse_value(doc["i_"], i));

        unsigned u = 0;
        REQUIRE(json_utils::try_parse_value(doc["u"], u));
        REQUIRE(u == 42);
        REQUIRE_FALSE(json_utils::try_parse_value(doc["u_"], u));

        float ff = 0.f;
        REQUIRE(json_utils::try_parse_value(doc["f0"], ff));
        REQUIRE(math::approximately(ff, 1.2f));
        REQUIRE(json_utils::try_parse_value(doc["f1"], ff));
        REQUIRE(math::approximately(ff, 2.f));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["f_"], ff));

        vector<int> vv;
        REQUIRE(json_utils::try_parse_value(doc["vv"], vv));
        REQUIRE(vv == vector<int>{1,2,2,3});
        REQUIRE_FALSE(json_utils::try_parse_value(doc["vv_"], vv));

        hash_set<int> hs;
        REQUIRE(json_utils::try_parse_value(doc["vv"], hs));
        REQUIRE(hs == hash_set<int>{1,2,3});
        REQUIRE_FALSE(json_utils::try_parse_value(doc["vv_"], hs));

        hash_multiset<int> hms;
        REQUIRE(json_utils::try_parse_value(doc["vv"], hms));
        REQUIRE(hms == hash_multiset<int>{1,2,2,3});
        REQUIRE_FALSE(json_utils::try_parse_value(doc["vv_"], hms));

        flat_set<int> fs;
        REQUIRE(json_utils::try_parse_value(doc["vv"], fs));
        REQUIRE(fs == flat_set<int>{1,2,3});
        REQUIRE_FALSE(json_utils::try_parse_value(doc["vv_"], fs));

        flat_multiset<int> fms;
        REQUIRE(json_utils::try_parse_value(doc["vv"], fms));
        REQUIRE(fms == flat_multiset<int>{1,2,2,3});
        REQUIRE_FALSE(json_utils::try_parse_value(doc["vv_"], fms));
    }
    {
        str s0;
        wstr s1;
        str16 s2;
        str32 s3;
        str_hash s4;

        REQUIRE(json_utils::try_parse_value(doc["s0"], s0));
        REQUIRE(s0 == make_utf8("hello"));
        REQUIRE(json_utils::try_parse_value(doc["s0"], s1));
        REQUIRE(s1 == make_wide("hello"));
        REQUIRE(json_utils::try_parse_value(doc["s0"], s2));
        REQUIRE(s2 == make_utf16("hello"));
        REQUIRE(json_utils::try_parse_value(doc["s0"], s3));
        REQUIRE(s3 == make_utf32("hello"));
        REQUIRE(json_utils::try_parse_value(doc["s0"], s4));
        REQUIRE(s4 == make_hash("hello"));

        REQUIRE_FALSE(json_utils::try_parse_value(doc["s1"], s0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s1"], s1));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s1"], s2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s1"], s3));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s1"], s4));

        REQUIRE_FALSE(json_utils::try_parse_value(doc["s2"], s0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s2"], s1));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s2"], s2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s2"], s3));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["s2"], s4));
    }
    {
        t2f t2;
        REQUIRE(json_utils::try_parse_value(doc["t2"], t2));
        REQUIRE(t2 == make_trs2(v2f(1,2), 3.f, v2f(4,5)));

        t3f t3;
        REQUIRE(json_utils::try_parse_value(doc["t3"], t3));
        REQUIRE(t3 == make_trs3(v3f(1,2,3), v3f(4,5,6), v3f(8,9,10)));
    }
    {
        color c0, c1, c2;
        REQUIRE(json_utils::try_parse_value(doc["c0"], c0));
        REQUIRE(json_utils::try_parse_value(doc["c1"], c1));
        REQUIRE(json_utils::try_parse_value(doc["c2"], c2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["c0_"], c2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["c1_"], c2));
        REQUIRE(c0 == color(0.5f, 0.5f, 0.5f, 1.f));
        REQUIRE(c1 == color(0.1f, 1.0f, 0.2f, 1.0f));
        REQUIRE(c2 == color(0.1f, 0.2f, 0.3f, 0.4f));
    }
    {
        color32 c0, c1, c2;
        REQUIRE(json_utils::try_parse_value(doc["c32_0"], c0));
        REQUIRE(json_utils::try_parse_value(doc["c32_1"], c1));
        REQUIRE(json_utils::try_parse_value(doc["c32_2"], c2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["c32_0_"], c2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["c32_1_"], c2));
        REQUIRE(c0 == color32(20, 20, 20, 255));
        REQUIRE(c1 == color32(10, 255, 20, 255));
        REQUIRE(c2 == color32(10, 20, 30, 40));
    }
    {
        b2i b0, b1;
        REQUIRE(json_utils::try_parse_value(doc["b2_0"], b0));
        REQUIRE(json_utils::try_parse_value(doc["b2_1"], b1));
        REQUIRE(b0 == b2i(1,0,2,0));
        REQUIRE(b1 == b2i(1,2,3,4));

        b2u b2;
        REQUIRE(json_utils::try_parse_value(doc["b2_1"], b2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["b2_2"], b2));

        b2u b3, b4;
        REQUIRE(json_utils::try_parse_value(doc["b2_3"], b3));
        REQUIRE(json_utils::try_parse_value(doc["b2_4"], b4));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["b2_5"], b4));
        REQUIRE(b3 == b2u(0,0,1,2));
        REQUIRE(b4 == b2u(1,2,3,4));
    }
    {
        b3i b0, b1;
        REQUIRE(json_utils::try_parse_value(doc["b3_0"], b0));
        REQUIRE(json_utils::try_parse_value(doc["b3_1"], b1));
        REQUIRE(b0 == b3i(1,0,0,2,0,0));
        REQUIRE(b1 == b3i(1,2,3,4,5,6));

        b3u b2;
        REQUIRE(json_utils::try_parse_value(doc["b3_1"], b2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["b3_2"], b2));

        b3u b3, b4;
        REQUIRE(json_utils::try_parse_value(doc["b3_3"], b3));
        REQUIRE(json_utils::try_parse_value(doc["b3_4"], b4));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["b3_5"], b4));
        REQUIRE(b3 == b3u(0,0,0,1,2,3));
        REQUIRE(b4 == b3u(1,2,3,4,5,6));
    }
    {
        m2f m0, m1;
        REQUIRE(json_utils::try_parse_value(doc["m22"], m0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m33"], m0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m44"], m0));
        REQUIRE(m0 == m2f(1,2,3,4));
        REQUIRE(json_utils::try_parse_value(doc["m22_"], m1));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m33_"], m1));
        REQUIRE(m0 == m1);
    }
    {
        m3f m0, m1;
        REQUIRE(json_utils::try_parse_value(doc["m33"], m0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m22"], m0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m44"], m0));
        REQUIRE(m0 == m3f(1,2,3,3,4,5,5,6,7));
        REQUIRE(json_utils::try_parse_value(doc["m33_"], m1));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m22_"], m1));
        REQUIRE(m0 == m1);
    }
    {
        m4f m0, m1;
        REQUIRE(json_utils::try_parse_value(doc["m44"], m0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m22"], m0));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m33"], m0));
        REQUIRE(m0 == m4f(1,2,3,4,3,4,5,6,5,6,7,8,7,8,9,0));
        REQUIRE(json_utils::try_parse_value(doc["m44_"], m1));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["m33_"], m1));
        REQUIRE(m0 == m1);
    }
    {
        v2f v0;
        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(v0 == v2f(42.f));
        REQUIRE(json_utils::try_parse_value(doc["v0f"], v0));
        REQUIRE(v0 == v2f(42.2f));
        REQUIRE(json_utils::try_parse_value(doc["v5f"], v0));
        REQUIRE(v0 == v2f(1.1f, 2.2f));
    }
    {
        v3f v0;
        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(v0 == v3f(42.f));
        REQUIRE(json_utils::try_parse_value(doc["v0f"], v0));
        REQUIRE(v0 == v3f(42.2f));
        REQUIRE(json_utils::try_parse_value(doc["v5f"], v0));
        REQUIRE(v0 == v3f(1.1f, 2.2f, 3.3f));
    }
    {
        v4f v0;
        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(v0 == v4f(42.f));
        REQUIRE(json_utils::try_parse_value(doc["v0f"], v0));
        REQUIRE(v0 == v4f(42.2f));
        REQUIRE(json_utils::try_parse_value(doc["v5f"], v0));
        REQUIRE(v0 == v4f(1.1f, 2.2f, 3.3f, 4.4f));
    }
    {
        v2u v0;
        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(v0 == v2u(42));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v1"], v0));
        REQUIRE(json_utils::try_parse_value(doc["v3"], v0));
        REQUIRE(v0 == v2u(1,2));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v6"], v0));
    }
    {
        v3u v0;
        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(v0 == v3u(42));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v1"], v0));
        REQUIRE(json_utils::try_parse_value(doc["v4"], v0));
        REQUIRE(v0 == v3u(1,2,3));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v6"], v0));
    }
    {
        v4u v0;
        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(v0 == v4u(42));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v1"], v0));
        REQUIRE(json_utils::try_parse_value(doc["v5"], v0));
        REQUIRE(v0 == v4u(1,2,3,4));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v6"], v0));
    }
    {
        v2i v0, v1, v2, v3, v4, v5, v6;

        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(json_utils::try_parse_value(doc["v1"], v1));
        REQUIRE(json_utils::try_parse_value(doc["v2"], v2));
        REQUIRE(json_utils::try_parse_value(doc["v3"], v3));
        REQUIRE(json_utils::try_parse_value(doc["v4"], v4));
        REQUIRE(json_utils::try_parse_value(doc["v5"], v5));
        REQUIRE(json_utils::try_parse_value(doc["v6"], v6));

        REQUIRE(v0 == v2i(42));
        REQUIRE(v1 == v2i(-42));
        REQUIRE(v2 == v2i(1,0));
        REQUIRE(v3 == v2i(1,2));
        REQUIRE(v4 == v2i(1,2));
        REQUIRE(v5 == v2i(1,2));
        REQUIRE(v6 == v2i(1,-2));

        v2i v2_, v3_, v4_, v5_, v6_;

        REQUIRE_FALSE(json_utils::try_parse_value(doc["v2_"], v2_));
        REQUIRE(json_utils::try_parse_value(doc["v3_"], v3_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v4_"], v4_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v5_"], v5_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v6_"], v6_));

        REQUIRE(v3 == v3_);
    }
    {
        v3i v0, v1, v2, v3, v4, v5, v6;

        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(json_utils::try_parse_value(doc["v1"], v1));
        REQUIRE(json_utils::try_parse_value(doc["v2"], v2));
        REQUIRE(json_utils::try_parse_value(doc["v3"], v3));
        REQUIRE(json_utils::try_parse_value(doc["v4"], v4));
        REQUIRE(json_utils::try_parse_value(doc["v5"], v5));
        REQUIRE(json_utils::try_parse_value(doc["v6"], v6));

        REQUIRE(v0 == v3i(42));
        REQUIRE(v1 == v3i(-42));
        REQUIRE(v2 == v3i(1,0,0));
        REQUIRE(v3 == v3i(1,2,0));
        REQUIRE(v4 == v3i(1,2,3));
        REQUIRE(v5 == v3i(1,2,3));
        REQUIRE(v6 == v3i(1,-2,3));

        v3i v2_, v3_, v4_, v5_, v6_;

        REQUIRE_FALSE(json_utils::try_parse_value(doc["v2_"], v2_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v3_"], v3_));
        REQUIRE(json_utils::try_parse_value(doc["v4_"], v4_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v5_"], v5_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v6_"], v6_));

        REQUIRE(v4 == v4_);
    }
    {
        v4i v0, v1, v2, v3, v4, v5, v6;

        REQUIRE(json_utils::try_parse_value(doc["v0"], v0));
        REQUIRE(json_utils::try_parse_value(doc["v1"], v1));
        REQUIRE(json_utils::try_parse_value(doc["v2"], v2));
        REQUIRE(json_utils::try_parse_value(doc["v3"], v3));
        REQUIRE(json_utils::try_parse_value(doc["v4"], v4));
        REQUIRE(json_utils::try_parse_value(doc["v5"], v5));
        REQUIRE(json_utils::try_parse_value(doc["v6"], v6));

        REQUIRE(v0 == v4i(42));
        REQUIRE(v1 == v4i(-42));
        REQUIRE(v2 == v4i(1,0,0,0));
        REQUIRE(v3 == v4i(1,2,0,0));
        REQUIRE(v4 == v4i(1,2,3,0));
        REQUIRE(v5 == v4i(1,2,3,4));
        REQUIRE(v6 == v4i(1,-2,3,-4));

        v4i v2_, v3_, v4_, v5_, v6_;

        REQUIRE_FALSE(json_utils::try_parse_value(doc["v2_"], v2_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v3_"], v3_));
        REQUIRE_FALSE(json_utils::try_parse_value(doc["v4_"], v4_));
        REQUIRE(json_utils::try_parse_value(doc["v5_"], v5_));
        REQUIRE(json_utils::try_parse_value(doc["v6_"], v6_));

        REQUIRE(v5 == v5_);
        REQUIRE(v6 == v6_);
    }
    {
        image_file_format e0 = image_file_format::dds;
        image_data_format e1 = image_data_format::a8;

        REQUIRE_FALSE(json_utils::try_parse_value(doc["e0_"], e0));
        REQUIRE(e0 == image_file_format::dds);
        REQUIRE(json_utils::try_parse_value(doc["e0"], e0));
        REQUIRE(e0 == image_file_format::pvr);

        REQUIRE_FALSE(json_utils::try_parse_value(doc["e1_"], e1));
        REQUIRE(e1 == image_data_format::a8);
        REQUIRE(json_utils::try_parse_value(doc["e1"], e1));
        REQUIRE(e1 == image_data_format::rgb_etc1);
    }
}
