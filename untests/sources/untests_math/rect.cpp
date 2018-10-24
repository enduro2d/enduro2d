/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("rect") {
    {
        REQUIRE(r4i().position == v2i(0,0));
        REQUIRE(r4i().size == v2i(0,0));

        REQUIRE(r4i(1,2).position == v2i(0,0));
        REQUIRE(r4i(1,2).size == v2i(1,2));

        REQUIRE(r4i(1,2,3,4).position == v2i(1,2));
        REQUIRE(r4i(1,2,3,4).size == v2i(3,4));

        REQUIRE(r4i(v2i{1,2}).position == v2i(0,0));
        REQUIRE(r4i(v2i{1,2}).size == v2i(1,2));

        REQUIRE(r4i(v2i{1,2},v2i{3,4}).position == v2i(1,2));
        REQUIRE(r4i(v2i{1,2},v2i{3,4}).size == v2i(3,4));
    }
    {
        REQUIRE(r4f(1,2,3,4).cast_to<i32>() == r4i(1,2,3,4));
    }
    {
        REQUIRE(make_rect(2,1) == r4i(0,0,2,1));
        REQUIRE(make_rect(4,3,2,1) == r4i(4,3,2,1));
        REQUIRE(make_rect(v2i{2,1}) == r4i(0,0,2,1));
        REQUIRE(make_rect(v2i{4,3},v2i{2,1}) == r4i(4,3,2,1));
    }
    {
        auto r0 = r4i(1,2,3,4);
        auto r1 = r4i(r0);
        REQUIRE(r1 == r4i(1,2,3,4));
        r1 = r4i(4,3,2,1);
        REQUIRE(r1 == r4i(4,3,2,1));
    }
    {
        REQUIRE(r4i(1,2,3,4) == r4i(1,2,3,4));
        REQUIRE_FALSE(r4i(1,2,3,4) == r4i(1,2,4,3));
        REQUIRE_FALSE(r4i(1,2,3,4) == r4i(2,1,3,4));
        REQUIRE_FALSE(r4i(1,2,3,4) == r4i(2,1,4,3));

        REQUIRE_FALSE(r4i(1,2,3,4) != r4i(1,2,3,4));
        REQUIRE(r4i(1,2,3,4) != r4i(1,2,4,3));
        REQUIRE(r4i(1,2,3,4) != r4i(2,1,3,4));
        REQUIRE(r4i(1,2,3,4) != r4i(2,1,4,3));
    }
    {
        REQUIRE(r4i(4,4) < r4i(4,5));
        REQUIRE_FALSE(r4i(4,4) < r4i(3,4));
        REQUIRE_FALSE(r4i(4,4) < r4i(4,3));
    }
    {
        REQUIRE(math::approximately(r4i(1,2,3,4), r4i(1,2,4,5), 1));
        REQUIRE_FALSE(math::approximately(r4i(1,2,3,4), r4i(1,2,4,5)));
    }
    {
        REQUIRE(math::minimum(r4i(1,2,3,4)) == v2i(1,2));
        REQUIRE(math::minimum(r4i(1,2,-3,5)) == v2i(-2,2));
        REQUIRE(math::minimum(r4i(1,2,3,-5)) == v2i(1,-3));
        REQUIRE(math::minimum(r4i(1,2,-3,-5)) == v2i(-2,-3));

        REQUIRE(math::maximum(r4i(1,2,3,4)) == v2i(4,6));
        REQUIRE(math::maximum(r4i(1,2,-3,5)) == v2i(1,7));
        REQUIRE(math::maximum(r4i(1,2,3,-5)) == v2i(4,2));
        REQUIRE(math::maximum(r4i(1,2,-3,-5)) == v2i(1,2));

        REQUIRE(math::area(r4i(1,2,3,4)) == 12);
        REQUIRE(math::area(r4i(1,2,3,-4)) == -12);
        REQUIRE(math::area(r4i(1,2,-3,4)) == -12);
        REQUIRE(math::area(r4i(1,2,-3,-4)) == 12);

        REQUIRE(math::abs_area(r4i(1,2,3,4)) == 12);
        REQUIRE(math::abs_area(r4i(1,2,3,-4)) == 12);
        REQUIRE(math::abs_area(r4i(1,2,-3,4)) == 12);
        REQUIRE(math::abs_area(r4i(1,2,-3,-4)) == 12);

        REQUIRE(math::merged(r4i(1,2,3,4), r4i(1,2,3,4)) == r4i(1,2,3,4));
        REQUIRE(math::merged(r4i(1,2,3,4), r4i(0,1,3,4)) == r4i(0,1,4,5));
        REQUIRE(math::merged(r4i(1,2,3,4), r4i(1,2,4,5)) == r4i(1,2,4,5));
        REQUIRE(math::merged(r4i(1,2,3,4), r4i(0,1,4,5)) == r4i(0,1,4,5));

        REQUIRE(math::merged(r4i(1,2,3,4), r4i(1,2,-3,-5)) == r4i(-2,-3,6,9));
        REQUIRE(math::merged(r4i(-1,-2,3,4), r4i(1,2,-3,-5)) == r4i(-2,-3,4,5));

        REQUIRE(math::inside(r4i(1,2,3,4), v2i(1,2)));
        REQUIRE(math::inside(r4i(1,2,3,4), v2i(4,5)));
        REQUIRE_FALSE(math::inside(r4i(1,2,3,4), v2i(4,7)));
        REQUIRE_FALSE(math::inside(r4i(1,2,3,4), v2i(0,5)));

        REQUIRE(math::inside(r4i(1,2,-3,-4), v2i(1,2)));
        REQUIRE(math::inside(r4i(1,2,-3,-4), v2i(-1,-2)));
        REQUIRE_FALSE(math::inside(r4i(1,2,-3,-4), v2i(2,2)));
        REQUIRE_FALSE(math::inside(r4i(1,2,-3,-4), v2i(1,3)));
        REQUIRE_FALSE(math::inside(r4i(1,2,-3,-4), v2i(-3,2)));
        REQUIRE_FALSE(math::inside(r4i(1,2,-3,-4), v2i(1,-3)));

        REQUIRE_FALSE(math::contains_nan(r4i(1,2,3,4)));
        REQUIRE_FALSE(math::contains_nan(r4f(1.f,2.f,3.f,4.f)));
        REQUIRE(math::contains_nan(r4f(1.f,std::numeric_limits<f32>::quiet_NaN())));
        REQUIRE(math::contains_nan(r4f(std::numeric_limits<f32>::infinity(), 1.f)));
    }
}
