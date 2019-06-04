/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("aabb") {
    {
        REQUIRE(b3i::zero() == b3i(0,0,0,0,0,0));
        REQUIRE(b3i::unit() == b3i(0,0,0,1,1,1));
    }
    {
        REQUIRE(b3i().position == v3i(0,0,0));
        REQUIRE(b3i().size == v3i(0,0,0));

        REQUIRE(b3i(1,2,3).position == v3i(0,0,0));
        REQUIRE(b3i(1,2,3).size == v3i(1,2,3));

        REQUIRE(b3i(1,2,3,4,5,6).position == v3i(1,2,3));
        REQUIRE(b3i(1,2,3,4,5,6).size == v3i(4,5,6));

        REQUIRE(b3i(v3i{1,2,3}).position == v3i(0,0,0));
        REQUIRE(b3i(v3i{1,2,3}).size == v3i(1,2,3));

        REQUIRE(b3i(v3i{1,2,3},v3i{3,4,5}).position == v3i(1,2,3));
        REQUIRE(b3i(v3i{1,2,3},v3i{3,4,5}).size == v3i(3,4,5));
    }
    {
        REQUIRE(b3f(1,2,3,4,5,6).cast_to<i32>() == b3i(1,2,3,4,5,6));
    }
    {
        auto r0 = b3i(1,2,3,4,5,6);
        REQUIRE(r0.data()[0] == 1);
        REQUIRE(r0.data()[1] == 2);
        REQUIRE(r0.data()[2] == 3);
        REQUIRE(r0.data()[3] == 4);
        REQUIRE(r0.data()[4] == 5);
        REQUIRE(r0.data()[5] == 6);
        r0.data()[0] = 2;
        REQUIRE(r0 == b3i(2,2,3,4,5,6));
        const auto& cr0 = r0;
        REQUIRE(cr0.data()[0] == 2);
        REQUIRE(cr0.data()[1] == 2);
        REQUIRE(cr0.data()[2] == 3);
        REQUIRE(cr0.data()[3] == 4);
        REQUIRE(cr0.data()[4] == 5);
        REQUIRE(cr0.data()[5] == 6);
    }
    {
        auto r0 = b3i(1,2,3,4,5,6);
        REQUIRE(r0[0] == 1);
        REQUIRE(r0[1] == 2);
        REQUIRE(r0[2] == 3);
        REQUIRE(r0[3] == 4);
        REQUIRE(r0[4] == 5);
        REQUIRE(r0[5] == 6);
        r0[0] = 2;
        REQUIRE(r0 == b3i(2,2,3,4,5,6));
        const auto& cr0 = r0;
        REQUIRE(cr0[0] == 2);
        REQUIRE(cr0[1] == 2);
        REQUIRE(cr0[2] == 3);
        REQUIRE(cr0[3] == 4);
        REQUIRE(cr0[4] == 5);
        REQUIRE(cr0[5] == 6);
    }
    {
        auto r0 = b3i(1,2,3,4,5,6);
        REQUIRE(&r0 == &(r0 += 1));
        REQUIRE(r0 == b3i(2,3,4,4,5,6));
        REQUIRE(&r0 == &(r0 -= 4));
        REQUIRE(r0 == b3i(-2,-1,0,4,5,6));
        REQUIRE(&r0 == &(r0 *= 2));
        REQUIRE(r0 == b3i(-2,-1,0,8,10,12));
        REQUIRE(&r0 == &(r0 /= 3));
        REQUIRE(r0 == b3i(-2,-1,0,2,3,4));

        REQUIRE(b3i(1,2,3,4,5,6) + 2 == b3i(3,4,5,4,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) - 2 == b3i(-1,0,1,4,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) * 2 == b3i(1,2,3,8,10,12));
        REQUIRE(b3i(1,2,3,4,5,6) / 2 == b3i(1,2,3,2,2,3));

        REQUIRE(b3i(1,2,3,4,5,6) + v3i(1,2,3) == b3i(2,4,6,4,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) - v3i(3,2,1) == b3i(-2,0,2,4,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) * v3i(2,3,4) == b3i(1,2,3,8,15,24));
        REQUIRE(b3i(1,2,3,4,12,20) / v3i(2,3,4) == b3i(1,2,3,2,4,5));
    }
    {
        REQUIRE(make_aabb(2,1,0) == b3i(0,0,0,2,1,0));
        REQUIRE(make_aabb(4,3,2,1,0,-1) == b3i(4,3,2,1,0,-1));
        REQUIRE(make_aabb(v3i{2,1,0}) == b3i(0,0,0,2,1,0));
        REQUIRE(make_aabb(v3i{4,3,2},v3i{2,1,0}) == b3i(4,3,2,2,1,0));

        REQUIRE(math::make_minmax_aabb(1,2,3,4,5,6) == b3i(1,2,3,3,3,3));
        REQUIRE(math::make_minmax_aabb(3,4,1,2,3,4) == b3i(2,3,1,1,1,3));
    }
    {
        auto r0 = b3i(1,2,3,4,5,6);
        auto r1 = b3i(r0);
        REQUIRE(r1 == b3i(1,2,3,4,5,6));
        r1 = b3i(4,3,2,1,0,-1);
        REQUIRE(r1 == b3i(4,3,2,1,0,-1));
    }
    {
        REQUIRE(b3i(1,2,3,4,5,6) == b3i(1,2,3,4,5,6));
        REQUIRE_FALSE(b3i(1,2,3,4,5,6) == b3i(1,2,4,3,5,6));
        REQUIRE_FALSE(b3i(1,2,3,4,5,6) == b3i(2,1,3,4,5,6));
        REQUIRE_FALSE(b3i(1,2,3,4,5,6) == b3i(2,1,4,3,5,6));

        REQUIRE_FALSE(b3i(1,2,3,4,5,6) != b3i(1,2,3,4,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) != b3i(1,2,4,3,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) != b3i(2,1,3,4,5,6));
        REQUIRE(b3i(1,2,3,4,5,6) != b3i(2,1,4,3,5,6));
    }
    {
        REQUIRE(b3i(4,4,1) < b3i(4,5,1));
        REQUIRE_FALSE(b3i(4,4,1) < b3i(3,4,1));
        REQUIRE_FALSE(b3i(4,4,1) < b3i(4,3,1));
    }
    {
        REQUIRE(math::approximately(b3i(1,2,3,4,1,2), b3i(1,2,4,5,1,2), 1));
        REQUIRE_FALSE(math::approximately(b3i(1,2,3,4,1,2), b3i(1,2,4,5,1,2)));
    }
    {
        REQUIRE(math::minimum(b3i(1,2,3,4,5,6)) == v3i(1,2,3));
        REQUIRE(math::maximum(b3i(1,2,3,4,5,6)) == v3i(5,7,9));

        REQUIRE(math::volume(b3i(1,2,3,4,5,6)) == 120);
        REQUIRE(math::volume(b3i(1,2,3,-4,5,6)) == -120);
        REQUIRE(math::volume(b3i(1,2,3,4,-5,6)) == -120);
        REQUIRE(math::volume(b3i(1,2,3,-4,5,-6)) == 120);

        REQUIRE(math::abs_volume(b3i(1,2,3,4,5,6)) == 120);
        REQUIRE(math::abs_volume(b3i(1,2,3,-4,5,6)) == 120);
        REQUIRE(math::abs_volume(b3i(1,2,3,4,-5,6)) == 120);
        REQUIRE(math::abs_volume(b3i(1,2,3,-4,5,-6)) == 120);

        REQUIRE(math::merged(b3i(1,2,3,4,5,6), b3i(1,2,3,4,5,6)) == b3i(1,2,3,4,5,6));
        REQUIRE(math::merged(b3i(1,2,3,4,5,6), b3i(0,2,3,4,5,6)) == b3i(0,2,3,5,5,6));
        REQUIRE(math::merged(b3i(1,2,3,4,5,6), b3i(1,2,3,4,5,7)) == b3i(1,2,3,4,5,7));

        REQUIRE(math::inside(b3i(1,2,3,4,5,6), v3i(1,2,3)));
        REQUIRE(math::inside(b3i(1,2,3,4,5,6), v3i(4,5,6)));
        REQUIRE_FALSE(math::inside(b3i(1,2,3,4,5,6), v3i(3,4,10)));
        REQUIRE_FALSE(math::inside(b3i(1,2,3,4,5,6), v3i(0,3,5)));

        REQUIRE_FALSE(math::overlaps(b3i(0,0,0,10,10,10), b3i(10,0,0,10,10,10)));
        REQUIRE_FALSE(math::overlaps(b3i(0,0,0,10,10,10), b3i(0,0,10,10,10,10)));
        REQUIRE_FALSE(math::overlaps(b3i(0,0,0,10,10,10), b3i(10,10,0,10,10,10)));
        REQUIRE(math::overlaps(b3i(0,0,0,10,10,10), b3i(9,0,0,10,10,10)));
        REQUIRE(math::overlaps(b3i(0,0,0,10,10,10), b3i(0,9,0,10,10,10)));
        REQUIRE(math::overlaps(b3i(0,0,0,10,10,10), b3i(9,9,0,10,10,10)));
        REQUIRE(math::overlaps(b3i(0,0,0,10,10,10), b3i(-9,0,0,10,10,10)));
        REQUIRE(math::overlaps(b3i(0,0,0,10,10,10), b3i(0,-9,0,10,10,10)));
        REQUIRE(math::overlaps(b3i(0,0,0,10,10,10), b3i(-9,-9,0,10,10,10)));

        REQUIRE_FALSE(math::contains_nan(b3i(1,2,3,4,5,6)));
        REQUIRE_FALSE(math::contains_nan(b3f(1.f,2.f,3.f,4.f,5.f,6.f)));
        REQUIRE(math::contains_nan(b3f(1.f,2.f,std::numeric_limits<f32>::quiet_NaN())));
        REQUIRE(math::contains_nan(b3f(std::numeric_limits<f32>::infinity(), 1.f,2.f)));
    }
    {
        REQUIRE(math::normalized_to_point(b3f(10.f, 20.f, 30.f), v3f(0.f, 0.f, 0.f)) == v3f(0.f, 0.f, 0.f));
        REQUIRE(math::normalized_to_point(b3f(10.f, 20.f, 30.f), v3f(0.5f, 0.5f, 0.5f)) == v3f(5.f, 10.f, 15.f));
        REQUIRE(math::normalized_to_point(b3f(10.f, 20.f, 30.f), v3f(0.25f, 0.75f, 0.75f)) == v3f(2.5f, 15.f, 22.5f));
        REQUIRE(math::normalized_to_point(b3f(10.f, 20.f, 30.f), v3f(1.f, 1.f, 1.f)) == v3f(10.f, 20.f, 30.f));
    }
    {
        REQUIRE(math::point_to_normalized(b3f(10.f, 20.f, 30.f), v3f(0.f, 0.f, 0.f)) == v3f(0.f, 0.f, 0.f));
        REQUIRE(math::point_to_normalized(b3f(10.f, 20.f, 30.f), v3f(5.f, 10.f, 15.f)) == v3f(0.5f, 0.5f, 0.5f));
        REQUIRE(math::point_to_normalized(b3f(10.f, 20.f, 30.f), v3f(2.5f, 15.f, 22.5f)) == v3f(0.25f, 0.75f, 0.75f));
        REQUIRE(math::point_to_normalized(b3f(10.f, 20.f, 30.f), v3f(10.f, 20.f, 30.f)) == v3f(1.f, 1.f, 1.f));
    }
}
