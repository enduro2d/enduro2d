/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("vec2") {
    {
        REQUIRE(v2i::zero() == v2i(0));
        REQUIRE(v2i::unit() == v2i(1));
        REQUIRE(v2i::unit_x() == v2i(1,0));
        REQUIRE(v2i::unit_y() == v2i(0,1));
    }
    {
        static_assert(std::is_same<
            vec2<i32>, v2i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, v2i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v2i>().x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v2i>().y)
        >::value, "static unit test error");

        static_assert(std::is_same<
            vec2<f32>, v2f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, v2f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v2f>().x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v2f>().y)
        >::value, "static unit test error");
    }
    {
        auto v0 = v2i();
        auto v1 = v2i(1);
        auto v2 = v2i(2,3);
        auto v3 = v2i(v3i(4,5,6));
        auto v4 = v2i(v4i(4,5,6,7));
        REQUIRE((v0.x == 0 && v0.y == 0));
        REQUIRE((v1.x == 1 && v1.y == 1));
        REQUIRE((v2.x == 2 && v2.y == 3));
        REQUIRE((v3.x == 4 && v3.y == 5));
        REQUIRE((v4.x == 4 && v4.y == 5));
    }
    {
        REQUIRE(make_vec2(1) == v2i(1,1));
        REQUIRE(make_vec2(1,2) == v2i(1,2));
        REQUIRE(make_vec2(v3i(1,2,3)) == v2i(1,2));
        REQUIRE(make_vec2(v4i(1,2,3,4)) == v2i(1,2));
    }
    {
        auto v0 = v2i(1,2);
        auto v1 = v2i(v0);
        REQUIRE((v0.x == 1 && v0.y == 2));
        REQUIRE((v1.x == 1 && v1.y == 2));
        v1 = v2i(2,3);
        REQUIRE((v1.x == 2 && v1.y == 3));
    }
    {
        REQUIRE(v2i(1,2) == v2i(1,2));
        REQUIRE_FALSE(v2i(1,2) == v2i(2,1));
        REQUIRE_FALSE(v2i(1,2) == v2i(1,1));

        REQUIRE_FALSE(v2i(1,2) != v2i(1,2));
        REQUIRE(v2i(1,2) != v2i(2,1));
        REQUIRE(v2i(1,2) != v2i(1,1));
    }
    {
        REQUIRE(v2i(2,1) < v2i(3,2));
        REQUIRE(v2i(2,1) < v2i(2,2));

        REQUIRE_FALSE(v2i(3,2) < v2i(2,1));
        REQUIRE_FALSE(v2i(2,2) < v2i(2,1));

        REQUIRE(v2i(2,1) <= v2i(3,2));
        REQUIRE(v2i(2,1) <= v2i(2,2));
        REQUIRE(v2i(2,1) <= v2i(2,1));
        REQUIRE(v2i(1,2) <= v2i(1,2));

        REQUIRE_FALSE(v2i(3,2) <= v2i(2,1));
        REQUIRE_FALSE(v2i(2,2) <= v2i(2,1));

        REQUIRE(v2i(3,2) >= v2i(2,1));
        REQUIRE(v2i(2,2) >= v2i(2,1));
        REQUIRE(v2i(2,1) >= v2i(2,1));
        REQUIRE(v2i(1,2) >= v2i(1,2));

        REQUIRE_FALSE(v2i(2,1) >= v2i(3,2));
        REQUIRE_FALSE(v2i(2,1) >= v2i(2,2));
    }
    {
        REQUIRE((v2i(1,2)[0] == 1 && v2i(1,2)[1] == 2));
        REQUIRE((v2i(1,2).data()[0] == 1 && v2i(1,2).data()[1] == 2));
        v2i v = v2i::unit();
        v[0] = 3;
        v[1] = 4;
        REQUIRE(v == v2i{3,4});
    }
    {
        REQUIRE(-v2i(2,3) == v2i(-2,-3));

        REQUIRE(3 + v2i(2,3) == v2i(5, 6));
        REQUIRE(7 - v2i(4,5) == v2i(3, 2));
        REQUIRE(2.f * v2f(2,3) == v2f(4, 6));
        REQUIRE(8u / v2u(8,4) == v2u(1, 2));

        REQUIRE(v2i(2,3) + 3 == v2i(5, 6));
        REQUIRE(v2i(4,5) - 1 == v2i(3, 4));
        REQUIRE(v2f(2,3) * 2.f == v2f(4, 6));
        REQUIRE(v2u(8,4) / 4u == v2u(2, 1));

        REQUIRE(v2i(2,3) + v2i(2,3) == v2i(4, 6));
        REQUIRE(v2i(4,5) - v2i(2,4) == v2i(2, 1));
        REQUIRE(v2f(2,3) * v2f(4,3) == v2f(8, 9));
        REQUIRE(v2u(8,9) / v2u(4,3) == v2u(2, 3));
    }
    {
        REQUIRE(3.f + v2f(2) == v2f(5));
        REQUIRE(4.f - v2f(3) == v2f(1));
        REQUIRE(3.0 * v2d(2) == v2d(6));
        REQUIRE(6.0 / v2d(2) == v2d(3));

        REQUIRE(v2f(2) + 3.f == v2f(5));
        REQUIRE(v2f(3) - 2.f == v2f(1));
        REQUIRE(v2d(2) * 3.0 == v2d(6));
        REQUIRE(v2d(6) / 2.0 == v2d(3));

        REQUIRE(v2f(2) + v2f(3,4) == v2f(5,6));
        REQUIRE(v2f(3) - v2f(2,3) == v2f(1,0));
        REQUIRE(v2f(2) * v2f(3,2) == v2f(6,4));
        REQUIRE(v2f(6) / v2f(2,3) == v2f(3,2));
    }
    {
        v2i v = v2i::unit();
        REQUIRE((v += v2i(3,4)) == v2i(4, 5));
        REQUIRE((v -= v2i(1,3)) == v2i(3, 2));
        REQUIRE((v *= v2i(2,4)) == v2i(6, 8));
        REQUIRE((v /= v2i(3,2)) == v2i(2, 4));
    }
    {
        v2i v(1,2);
        REQUIRE((v += 2) == v2i(3, 4));
        REQUIRE((v -= 1) == v2i(2, 3));
        REQUIRE((v *= 2) == v2i(4, 6));
        REQUIRE((v /= 2) == v2i(2, 3));
    }
    {
        REQUIRE(math::approximately(v2i(1,2), v2i(2,1), 1));
        REQUIRE_FALSE(math::approximately(v2i(1,2), v2i(2,1)));
    }
    {
        REQUIRE(math::dot(v2i(2,3), v2i(4,5)) == 23);
        REQUIRE(math::dot(v2i(2,3), v2i(4,-5)) == -7);
        REQUIRE(math::abs_dot(v2i(2,3), v2i(4,-5)) == 23);
        REQUIRE(math::approximately(math::dot(v2f(2,3), v2f(4,5)), 23.f));
    }
    {
        REQUIRE(math::length_squared(v2i(0,0)) ==  0);
        REQUIRE(math::length_squared(v2i(2,0)) ==  4);
        REQUIRE(math::length_squared(v2u(0,3)) ==  9);
        REQUIRE(math::length_squared(v2u(2,3)) == 13);

        REQUIRE(math::length(v2f(0,0)) == 0.f);
        REQUIRE(math::length(v2f(2,0)) == 2.f);
        REQUIRE(math::length(v2f(0,3)) == 3.f);
    }
    {
        REQUIRE(math::distance_squared(v2i(0,0), v2i(0,0)) ==  0);
        REQUIRE(math::distance_squared(v2i(4,0), v2i(0,0)) == 16);
        REQUIRE(math::distance_squared(v2i(8,0), v2i(2,0)) == 36);
        REQUIRE(math::distance_squared(v2u(0,0), v2u(0,4)) == 16);
        REQUIRE(math::distance_squared(v2u(0,2), v2u(0,8)) == 36);
        REQUIRE(math::distance_squared(v2u(0,4), v2u(0,0)) == 16);
        REQUIRE(math::distance_squared(v2u(0,8), v2u(0,2)) == 36);

        REQUIRE(math::distance(v2f(0,0), v2f(0,0)) == 0.f);
        REQUIRE(math::distance(v2f(4,0), v2f(0,0)) == 4.f);
        REQUIRE(math::distance(v2f(8,0), v2f(2,0)) == 6.f);
        REQUIRE(math::distance(v2f(0,0), v2f(0,4)) == 4.f);
        REQUIRE(math::distance(v2f(0,2), v2f(0,8)) == 6.f);

        REQUIRE(math::distance(v2f(5,7), v2f(2,3)) == 5.f);
        REQUIRE(math::distance(v2f(-5,7), v2f(-2,3)) == 5.f);
        REQUIRE(math::distance(v2f(5,-7), v2f(2,-3)) == 5.f);
        REQUIRE(math::distance(v2f(-5,-7), v2f(-2,-3)) == 5.f);
    }
    {
        v2f v0(2,0);
        v2f v1(0,2);
        REQUIRE(math::normalized(v0) == v2f(1,0));
        REQUIRE(math::normalized(v1) == v2f(0,1));
        REQUIRE(v0 == v2f(2,0));
        REQUIRE(v1 == v2f(0,2));

        REQUIRE(math::normalized(v2f(2,0)) == v2f(1,0));
        REQUIRE(math::normalized(v2f(0,2)) == v2f(0,1));
    }
    {
        REQUIRE(math::minimum(v2i(1,2)) == 1);
        REQUIRE(math::minimum(v2i(2,1)) == 1);

        REQUIRE(math::maximum(v2i(1,2)) == 2);
        REQUIRE(math::maximum(v2i(2,1)) == 2);
    }
    {
        v2i v0(3,4);

        REQUIRE(math::minimized(v0, v2i(2)) == v2i(2));
        REQUIRE(math::minimized(v0, v2i(3)) == v2i(3));
        REQUIRE(math::minimized(v0, v2i(4)) == v2i(3,4));
        REQUIRE(math::minimized(v0, v2i(1,2)) == v2i(1,2));

        REQUIRE(math::maximized(v0, v2i(5)) == v2i(5));
        REQUIRE(math::maximized(v0, v2i(4)) == v2i(4));
        REQUIRE(math::maximized(v0, v2i(3)) == v2i(3,4));
        REQUIRE(math::maximized(v0, v2i(3,4)) == v2i(3,4));
    }
    {
        v2i v0(3,4);
        REQUIRE(math::clamped(v0, v2i(0,0), v2i(2,3)) == v2i(2,3));
        REQUIRE(v0 == v2i(3,4));
        REQUIRE(math::clamped(v0, v2i(3,4), v2i(3,4)) == v2i(3,4));
        REQUIRE(v0 == v2i(3,4));

        REQUIRE(math::clamped(v0, v2i(0,0), v2i(0,0)) == v2i(0,0));
        REQUIRE(math::clamped(v0, v2i(0,0), v2i(3,2)) == v2i(3,2));
        REQUIRE(math::clamped(v0, v2i(0,0), v2i(4,3)) == v2i(3,3));

        REQUIRE(math::clamped(v0, v2i(4,5), v2i(9,9)) == v2i(4,5));
        REQUIRE(math::clamped(v0, v2i(6,5), v2i(9,9)) == v2i(6,5));
        REQUIRE(math::clamped(v0, v2i(7,6), v2i(9,9)) == v2i(7,6));

        REQUIRE(math::saturated(v2i(-1,-2)) == v2i(0,0));
        REQUIRE(math::saturated(v2i( 2, 3)) == v2i(1,1));
        REQUIRE(math::saturated(v2i(-1, 3)) == v2i(0,1));

        REQUIRE(math::saturated(v2f(-1, 0.5f)) == v2f(0,0.5f));
        REQUIRE(math::saturated(v2f(0.5f,2.f)) == v2f(0.5f,1.f));
    }
    {
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), 0.f) == v2f(1,2));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), 0.5f) == v2f(5.5f,11));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), 1.f) == v2f(10,20));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), 2.f) == v2f(19,38));

        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(0.f)) == v2f(1,2));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(0.5f)) == v2f(5.5f,11));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(1.f)) == v2f(10,20));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(2.f)) == v2f(19,38));

        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(0.f,1.f)) == v2f(1,20));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(0.5f,2.f)) == v2f(5.5f,38));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(1.f,0.5f)) == v2f(10,11));
        REQUIRE(math::lerp(v2f(1,2), v2f(10,20), v2f(2.f,0.f)) == v2f(19,2));

        REQUIRE(math::inverse_lerp(v2f(1,2), v2f(10,20), v2f(5.5f,11)) == v2f(0.5f));
        REQUIRE(math::inverse_lerp(v2f(1,2), v2f(10,20), v2f(5.5f,38)) == v2f(0.5f,2.f));
    }
}
