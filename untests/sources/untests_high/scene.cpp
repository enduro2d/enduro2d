/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

namespace
{
}

TEST_CASE("scene") {
    SECTION("ctor") {
        scene_iptr s = scene::create();
        REQUIRE(s);
        REQUIRE(s->root());
    }
}
