/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("ray3") {
    {
        REQUIRE(r3i::zero() == r3i(v3i::zero()));
        REQUIRE(r3i::unit_x() == r3i(v3i::unit_x()));
        REQUIRE(r3i::unit_y() == r3i(v3i::unit_y()));
        REQUIRE(r3i::unit_z() == r3i(v3i::unit_z()));
    }
    {
        static_assert(std::is_same<
            ray3<i32>, r3i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, r3i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec3<i32>, decltype(std::declval<r3i>().origin)
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec3<i32>, decltype(std::declval<r3i>().direction)
        >::value, "static unit test error");

        static_assert(std::is_same<
            ray3<f32>, r3f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, r3f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec3<f32>, decltype(std::declval<r3f>().origin)
        >::value, "static unit test error");
        static_assert(std::is_same<
            vec3<f32>, decltype(std::declval<r3f>().direction)
        >::value, "static unit test error");
    }
    {
        REQUIRE(r3i().origin == v3i(0,0,0));
        REQUIRE(r3i().direction == v3i(0,0,0));

        REQUIRE(r3i(1,2,3).origin == v3i(0,0,0));
        REQUIRE(r3i(1,2,3).direction == v3i(1,2,3));

        REQUIRE(r3i(v3i{1,2,3}).origin == v3i(0,0,0));
        REQUIRE(r3i(v3i{1,2,3}).direction == v3i(1,2,3));

        REQUIRE(r3i(1,2,3,3,4,5).origin == v3i(1,2,3));
        REQUIRE(r3i(1,2,3,3,4,5).direction == v3i(3,4,5));

        REQUIRE(r3i({1,2,3},{3,4,5}).origin == v3i(1,2,3));
        REQUIRE(r3i({1,2,3},{3,4,5}).direction == v3i(3,4,5));
    }
    {
        REQUIRE(r3f({1,2,3},{3,4,5}).cast_to<i32>() == r3i({1,2,3},{3,4,5}));
    }
    {
        auto r0 = r3i({1,2,3},{4,5,6});
        REQUIRE(r0.data()[0] == 1);
        REQUIRE(r0.data()[1] == 2);
        REQUIRE(r0.data()[2] == 3);
        REQUIRE(r0.data()[3] == 4);
        REQUIRE(r0.data()[4] == 5);
        REQUIRE(r0.data()[5] == 6);
        r0.data()[0] = 2;
        REQUIRE(r0 == r3i({2,2,3},{4,5,6}));
        const auto& cr0 = r0;
        REQUIRE(cr0.data()[0] == 2);
        REQUIRE(cr0.data()[1] == 2);
        REQUIRE(cr0.data()[2] == 3);
        REQUIRE(cr0.data()[3] == 4);
        REQUIRE(cr0.data()[4] == 5);
        REQUIRE(cr0.data()[5] == 6);
    }
    {
        auto r0 = r3i({1,2,3},{4,5,6});
        REQUIRE(r0[0] == 1);
        REQUIRE(r0[1] == 2);
        REQUIRE(r0[2] == 3);
        REQUIRE(r0[3] == 4);
        REQUIRE(r0[4] == 5);
        REQUIRE(r0[5] == 6);
        r0[0] = 2;
        REQUIRE(r0 == r3i({2,2,3},{4,5,6}));
        const auto& cr0 = r0;
        REQUIRE(cr0[0] == 2);
        REQUIRE(cr0[1] == 2);
        REQUIRE(cr0[2] == 3);
        REQUIRE(cr0[3] == 4);
        REQUIRE(cr0[4] == 5);
        REQUIRE(cr0[5] == 6);
    }
    {
        auto r0 = r3i({1,2,3},{4,5,6});
        REQUIRE(&r0 == &(r0 += 1));
        REQUIRE(r0 == r3i({2,3,4},{4,5,6}));
        REQUIRE(&r0 == &(r0 -= 4));
        REQUIRE(r0 == r3i({-2,-1,0},{4,5,6}));
        REQUIRE(&r0 == &(r0 *= 2));
        REQUIRE(r0 == r3i({-2,-1,0},{8,10,12}));
        REQUIRE(&r0 == &(r0 /= 3));
        REQUIRE(r0 == r3i({-2,-1,0},{2,3,4}));

        REQUIRE(r3i({1,2,3},{4,5,6}) + 2 == r3i({3,4,5},{4,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) - 2 == r3i({-1,0,1},{4,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) * 2 == r3i({1,2,3},{8,10,12}));
        REQUIRE(r3i({1,2,3},{4,5,6}) / 2 == r3i({1,2,3},{2,2,3}));

        REQUIRE(r3i({1,2,3},{4,5,6}) + v3i(1,2,3) == r3i({2,4,6},{4,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) - v3i(3,2,1) == r3i({-2,0,2},{4,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) * v3i(2,3,4) == r3i({1,2,3},{8,15,24}));
        REQUIRE(r3i({1,2,3},{4,12,20}) / v3i(2,3,4) == r3i({1,2,3},{2,4,5}));
    }
    {
        REQUIRE(make_ray3(2,1,0) == r3i(0,0,0,2,1,0));
        REQUIRE(make_ray3(4,3,2,2,1,0) == r3i({4,3,2},{2,1,0}));

        REQUIRE(make_ray3(v3i{2,1,0}) == r3i({0,0,0},{2,1,0}));
        REQUIRE(make_ray3(v3i{4,3,2},v3i{2,1,0}) == r3i({4,3,2},{2,1,0}));
    }
    {
        auto r0 = r3i({1,2,3},{4,5,6});
        auto r1 = r3i(r0);
        REQUIRE(r1 == r3i({1,2,3},{4,5,6}));
        r1 = r3i({4,3,2},{1,0,-1});
        REQUIRE(r1 == r3i({4,3,2},{1,0,-1}));
    }
    {
        REQUIRE(r3i({1,2,3},{4,5,6}) == r3i({1,2,3},{4,5,6}));
        REQUIRE_FALSE(r3i({1,2,3},{4,5,6}) == r3i({1,2,4},{3,5,6}));
        REQUIRE_FALSE(r3i({1,2,3},{4,5,6}) == r3i({2,1,3},{4,5,6}));
        REQUIRE_FALSE(r3i({1,2,3},{4,5,6}) == r3i({2,1,4},{3,5,6}));

        REQUIRE_FALSE(r3i({1,2,3},{4,5,6}) != r3i({1,2,3},{4,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) != r3i({1,2,4},{3,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) != r3i({2,1,3},{4,5,6}));
        REQUIRE(r3i({1,2,3},{4,5,6}) != r3i({2,1,4},{3,5,6}));
    }
    {
        REQUIRE(math::approximately(r3i({1,2,3},{4,1,2}), r3i({1,2,4},{5,1,2}), 1));
        REQUIRE_FALSE(math::approximately(r3i({1,2,3},{4,1,2}), r3i({1,2,4},{5,1,2})));
    }
    {
        REQUIRE_FALSE(math::contains_nan(r3i({1,2,3},{3,4,5})));
        REQUIRE_FALSE(math::contains_nan(r3f({1.f,2.f,3.f},{3.f,4.f,5.f})));
        REQUIRE(math::contains_nan(r3f(1.f,2.f,std::numeric_limits<f32>::quiet_NaN())));
        REQUIRE(math::contains_nan(r3f(std::numeric_limits<f32>::infinity(), 1.f,2.f)));
    }
}
