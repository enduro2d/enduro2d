/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_core.hpp"
using namespace e2d;

TEST_CASE("render"){
    SECTION("index_declaration"){
        index_declaration id;
        REQUIRE(id.index().type == index_declaration::index_type::unsigned_short);
        REQUIRE(id.index_size() == 2);

        index_declaration id2(index_declaration::index_type::unsigned_short);
        REQUIRE(id2.index().type == index_declaration::index_type::unsigned_short);
        REQUIRE(id2.index_size() == 2);

        index_declaration id3(index_declaration::index_type::unsigned_byte);
        REQUIRE(id3.index().type == index_declaration::index_type::unsigned_byte);
        REQUIRE(id3.index_size() == 1);

        REQUIRE(id == id2);
        REQUIRE_FALSE(id == id3);

        REQUIRE_FALSE(id != id2);
        REQUIRE(id != id3);

        index_declaration id4 = id;
        REQUIRE(id4 == id2);
        id4 = id3;
        REQUIRE(id4 != id2);
        REQUIRE(id4 == id3);
    }
    SECTION("vertex_declaration"){
        vertex_declaration vd;
        REQUIRE(vd.attribute_count() == 0);
        REQUIRE(vd.vertex_size() == 0);

        REQUIRE(&vd == &vd.add_attribute<v2f>("hello"));
        REQUIRE(vd.attribute_count() == 1);
        REQUIRE(vd.vertex_size() == 8);

        vertex_declaration::attribute_info ai = vd.attribute(0);
        REQUIRE(ai.name == "hello");
        REQUIRE(ai.columns == 2);
        REQUIRE(ai.stride == 0);
        REQUIRE(ai.type == vertex_declaration::attribute_type::floating_point);
        REQUIRE_FALSE(ai.normalized);

        REQUIRE(&vd == &vd.skip_bytes(4));
        REQUIRE(vd.vertex_size() == 12);

        REQUIRE(&vd == &vd.add_attribute<u16>("world").normalized());
        REQUIRE(vd.attribute_count() == 2);
        REQUIRE(vd.vertex_size() == 14);

        vertex_declaration::attribute_info ai2 = vd.attribute(1);
        REQUIRE(ai2.name == "world");
        REQUIRE(ai2.columns == 1);
        REQUIRE(ai2.stride == 12);
        REQUIRE(ai2.type == vertex_declaration::attribute_type::unsigned_short);
        REQUIRE(ai2.normalized);

        auto vd2 = vertex_declaration()
            .add_attribute("hello", 1, 2, vertex_declaration::attribute_type::floating_point, false)
            .skip_bytes(4)
            .add_attribute("world", 1, 1, vertex_declaration::attribute_type::unsigned_short, true);
        REQUIRE(vd == vd2);
        REQUIRE_FALSE(vd != vd2);

        auto vd3 = vertex_declaration()
            .add_attribute<v2f>("hello")
            .add_attribute<v2f>("world");
        REQUIRE_FALSE(vd == vd3);
        REQUIRE(vd != vd3);

        REQUIRE(vd3.vertex_size() == 16);
        vd3.skip_bytes(4);
        REQUIRE(vd3.vertex_size() == 20);

        vertex_declaration vd4 = vd2;
        REQUIRE(vd4 == vd);
        vd4 = vd3;
        REQUIRE(vd4 != vd);
        REQUIRE(vd4 == vd3);
    }
}
