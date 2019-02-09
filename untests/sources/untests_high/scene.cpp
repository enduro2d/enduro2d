/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

TEST_CASE("scene") {
    SECTION("ctor") {
        auto s = scene::create();
        REQUIRE(s);
        REQUIRE(s->root());

        {
            const_scene_iptr cs = s;
            REQUIRE(cs->root());
        }
    }
}
