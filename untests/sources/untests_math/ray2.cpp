/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("ray2") {
    {
        REQUIRE(r2i::zero() == r2i(v2i::zero()));
        REQUIRE(r2i::unit_x() == r2i(v2i::unit_x()));
        REQUIRE(r2i::unit_y() == r2i(v2i::unit_y()));
    }
    {
        static_assert(std::is_same<
            ray2<i32>, r2i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, r2i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec2<i32>, decltype(std::declval<r2i>().origin)
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec2<i32>, decltype(std::declval<r2i>().direction)
        >::value, "static unit test error");

        static_assert(std::is_same<
            ray2<f32>, r2f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, r2f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec2<f32>, decltype(std::declval<r2f>().origin)
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec2<f32>, decltype(std::declval<r2f>().direction)
        >::value, "static unit test error");
    }
    {
        REQUIRE(r2i().origin == v2i(0,0));
        REQUIRE(r2i().direction == v2i(0,0));

        REQUIRE(r2i(1,2).origin == v2i(0,0));
        REQUIRE(r2i(1,2).direction == v2i(1,2));

        REQUIRE(r2i({1,2}).origin == v2i(0,0));
        REQUIRE(r2i({1,2}).direction == v2i(1,2));

        REQUIRE(r2i(1,2,3,4).origin == v2i(1,2));
        REQUIRE(r2i(1,2,3,4).direction == v2i(3,4));

        REQUIRE(r2i({1,2},{3,4}).origin == v2i(1,2));
        REQUIRE(r2i({1,2},{3,4}).direction == v2i(3,4));
    }
    {
        REQUIRE(r2f({1,2},{3,4}).cast_to<i32>() == r2i({1,2},{3,4}));
    }
    {
        auto r0 = r2i({1,2},{3,4});
        REQUIRE(r0.data()[0] == 1);
        REQUIRE(r0.data()[1] == 2);
        REQUIRE(r0.data()[2] == 3);
        REQUIRE(r0.data()[3] == 4);
        r0.data()[0] = 2;
        REQUIRE(r0 == r2i({2,2},{3,4}));
        const auto& cr0 = r0;
        REQUIRE(cr0.data()[0] == 2);
        REQUIRE(cr0.data()[1] == 2);
        REQUIRE(cr0.data()[2] == 3);
        REQUIRE(cr0.data()[3] == 4);
    }
    {
        auto r0 = r2i({1,2},{3,4});
        REQUIRE(r0[0] == 1);
        REQUIRE(r0[1] == 2);
        REQUIRE(r0[2] == 3);
        REQUIRE(r0[3] == 4);
        r0[0] = 2;
        REQUIRE(r0 == r2i({2,2},{3,4}));
        const auto& cr0 = r0;
        REQUIRE(cr0[0] == 2);
        REQUIRE(cr0[1] == 2);
        REQUIRE(cr0[2] == 3);
        REQUIRE(cr0[3] == 4);
    }
    {
        auto r0 = r2i({1,2},{3,4});
        REQUIRE(&r0 == &(r0 += 1));
        REQUIRE(r0 == r2i({2,3},{3,4}));
        REQUIRE(&r0 == &(r0 -= 4));
        REQUIRE(r0 == r2i({-2,-1},{3,4}));
        REQUIRE(&r0 == &(r0 *= 2));
        REQUIRE(r0 == r2i({-2,-1},{6,8}));
        REQUIRE(&r0 == &(r0 /= 3));
        REQUIRE(r0 == r2i({-2,-1},{2,2}));

        REQUIRE(r2i({1,2},{3,4}) + 2 == r2i({3,4},{3,4}));
        REQUIRE(r2i({1,2},{3,4}) - 2 == r2i({-1,0},{3,4}));
        REQUIRE(r2i({1,2},{3,4}) * 3 == r2i({1,2},{9,12}));
        REQUIRE(r2i({1,2},{6,9}) / 3 == r2i({1,2},{2,3}));

        REQUIRE(r2i({1,2},{3,4}) + v2i(1,2) == r2i({2,4},{3,4}));
        REQUIRE(r2i({1,2},{3,4}) - v2i(1,2) == r2i({0,0},{3,4}));
        REQUIRE(r2i({1,2},{3,4}) * v2i(2,3) == r2i({1,2},{6,12}));
        REQUIRE(r2i({1,2},{6,8}) / v2i(3,2) == r2i({1,2},{2,4}));
    }
    {
        REQUIRE(make_ray2(2,1) == r2i(0,0,2,1));
        REQUIRE(make_ray2(4,3,2,1) == r2i({4,3},{2,1}));

        REQUIRE(make_ray2(v2i{2,1}) == r2i({0,0},{2,1}));
        REQUIRE(make_ray2(v2i{4,3},v2i{2,1}) == r2i({4,3},{2,1}));
    }
    {
        auto r0 = r2i({1,2},{3,4});
        auto r1 = r2i(r0);
        REQUIRE(r1 == r2i({1,2},{3,4}));
        r1 = r2i({4,3},{2,1});
        REQUIRE(r1 == r2i({4,3},{2,1}));
    }
    {
        REQUIRE(r2i({1,2},{3,4}) == r2i({1,2},{3,4}));
        REQUIRE_FALSE(r2i({1,2},{3,4}) == r2i({1,2},{4,3}));
        REQUIRE_FALSE(r2i({1,2},{3,4}) == r2i({2,1},{3,4}));
        REQUIRE_FALSE(r2i({1,2},{3,4}) == r2i({2,1},{4,3}));

        REQUIRE_FALSE(r2i({1,2},{3,4}) != r2i({1,2},{3,4}));
        REQUIRE(r2i({1,2},{3,4}) != r2i({1,2},{4,3}));
        REQUIRE(r2i({1,2},{3,4}) != r2i({2,1},{3,4}));
        REQUIRE(r2i({1,2},{3,4}) != r2i({2,1},{4,3}));
    }
    {
        REQUIRE(math::approximately(r2i({1,2},{3,4}), r2i({1,2},{4,5}), 1));
        REQUIRE_FALSE(math::approximately(r2i({1,2},{3,4}), r2i({1,2},{4,5})));
    }
    {
        REQUIRE_FALSE(math::contains_nan(r2i({1,2},{3,4})));
        REQUIRE_FALSE(math::contains_nan(r2f({1.f,2.f},{3.f,4.f})));
        REQUIRE(math::contains_nan(r2f({1.f,std::numeric_limits<f32>::quiet_NaN()})));
        REQUIRE(math::contains_nan(r2f({std::numeric_limits<f32>::infinity(), 1.f})));
    }
}
