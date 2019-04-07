/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

TEST_CASE("flipbook_player") {
    {
        flipbook_player fp;
        REQUIRE(math::approximately(fp.time(), 0.f));
        REQUIRE(math::approximately(fp.speed(), 1.f));
        REQUIRE_FALSE(fp.looped());
        REQUIRE(fp.stopped());
        REQUIRE_FALSE(fp.playing());
        REQUIRE(fp.sequence() == make_hash(""));
    }
    {
        flipbook_player fp;

        fp.playing(true);
        REQUIRE(fp.playing());
        REQUIRE_FALSE(fp.stopped());

        fp.stopped(true);
        REQUIRE_FALSE(fp.playing());
        REQUIRE(fp.stopped());

        fp.stopped(false);
        REQUIRE(fp.playing());
        REQUIRE_FALSE(fp.stopped());

        fp.playing(false);
        REQUIRE_FALSE(fp.playing());
        REQUIRE(fp.stopped());
    }
    {
        flipbook_player fp;

        REQUIRE(&fp.play(5.f) == &fp);
        REQUIRE(math::approximately(fp.time(), 5.f));
        REQUIRE(math::approximately(fp.speed(), 1.f));
        REQUIRE_FALSE(fp.looped());
        REQUIRE_FALSE(fp.stopped());
        REQUIRE(fp.playing());
        REQUIRE(fp.sequence() == make_hash(""));

        REQUIRE(&fp.play("hello") == &fp);
        REQUIRE(math::approximately(fp.time(), 0.f));
        REQUIRE(math::approximately(fp.speed(), 1.f));
        REQUIRE_FALSE(fp.looped());
        REQUIRE_FALSE(fp.stopped());
        REQUIRE(fp.playing());
        REQUIRE(fp.sequence() == make_hash("hello"));

        REQUIRE(&fp.stop(5.f) == &fp);
        REQUIRE(math::approximately(fp.time(), 5.f));
        REQUIRE(math::approximately(fp.speed(), 1.f));
        REQUIRE_FALSE(fp.looped());
        REQUIRE(fp.stopped());
        REQUIRE_FALSE(fp.playing());
        REQUIRE(fp.sequence() == make_hash("hello"));

        REQUIRE(&fp.stop("") == &fp);
        REQUIRE(math::approximately(fp.time(), 0.f));
        REQUIRE(math::approximately(fp.speed(), 1.f));
        REQUIRE_FALSE(fp.looped());
        REQUIRE(fp.stopped());
        REQUIRE_FALSE(fp.playing());
        REQUIRE(fp.sequence() == make_hash(""));
    }
}
