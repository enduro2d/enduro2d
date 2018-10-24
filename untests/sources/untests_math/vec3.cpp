/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("vec3") {
    {
        REQUIRE(v3i::zero() == v3i(0));
        REQUIRE(v3i::unit() == v3i(1));
        REQUIRE(v3i::unit_x() == v3i(1,0,0));
        REQUIRE(v3i::unit_y() == v3i(0,1,0));
        REQUIRE(v3i::unit_z() == v3i(0,0,1));
    }
    {
        static_assert(std::is_same<
            vec3<i32>, v3i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, v3i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v3i>().x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v3i>().y)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v3i>().z)
        >::value, "static unit test error");

        static_assert(std::is_same<
            vec3<f32>, v3f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, v3f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v3f>().x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v3f>().y)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v3f>().z)
        >::value, "static unit test error");
    }
    {
        auto v0 = v3i();
        auto v1 = v3i(1);
        auto v2 = v3i(1,2,3);
        auto v3 = v3i(v2i(3,4),5);
        auto v4 = v3i(v4i(3,4,5,6));
        REQUIRE((v0.x == 0 && v0.y == 0 && v0.z == 0));
        REQUIRE((v1.x == 1 && v1.y == 1 && v1.z == 1));
        REQUIRE((v2.x == 1 && v2.y == 2 && v2.z == 3));
        REQUIRE((v3.x == 3 && v3.y == 4 && v3.z == 5));
        REQUIRE((v4.x == 3 && v4.y == 4 && v4.z == 5));
    }
    {
        REQUIRE(make_vec3(1) == v3i(1,1,1));
        REQUIRE(make_vec3(1,2,3) == v3i(1,2,3));
        REQUIRE(make_vec3(v2i(1,2),3) == v3i(1,2,3));
        REQUIRE(make_vec3(v4i(1,2,3,4)) == v3i(1,2,3));
    }
    {
        auto v0 = v3i(1,2,3);
        auto v1 = v3i(v0);
        REQUIRE((v0.x == 1 && v0.y == 2 && v0.z == 3));
        REQUIRE((v1.x == 1 && v1.y == 2 && v1.z == 3));
        v1 = v3i(2,3,4);
        REQUIRE((v1.x == 2 && v1.y == 3 && v1.z == 4));
    }
    {
        REQUIRE(v3i(1,2,3) == v3i(1,2,3));
        REQUIRE_FALSE(v3i(1,2,3) == v3i(2,1,1));
        REQUIRE_FALSE(v3i(1,2,3) == v3i(1,1,1));

        REQUIRE_FALSE(v3i(1,2,3) != v3i(1,2,3));
        REQUIRE(v3i(1,2,3) != v3i(2,1,1));
        REQUIRE(v3i(1,2,3) != v3i(1,1,1));
    }
    {
        REQUIRE(v3i(2,1,0) < v3i(3,2,0));
        REQUIRE(v3i(2,1,0) < v3i(2,2,0));
        REQUIRE(v3i(2,1,0) < v3i(2,1,1));

        REQUIRE_FALSE(v3i(3,2,0) < v3i(2,1,0));
        REQUIRE_FALSE(v3i(2,2,0) < v3i(2,1,0));
        REQUIRE_FALSE(v3i(2,2,1) < v3i(2,2,0));

        REQUIRE(v3i(2,1,0) <= v3i(3,2,0));
        REQUIRE(v3i(2,1,0) <= v3i(2,2,0));
        REQUIRE(v3i(2,1,0) <= v3i(2,1,0));
        REQUIRE(v3i(1,2,0) <= v3i(1,2,0));

        REQUIRE_FALSE(v3i(3,2,0) <= v3i(2,1,0));
        REQUIRE_FALSE(v3i(2,2,0) <= v3i(2,1,0));

        REQUIRE(v3i(3,2,0) >= v3i(2,1,0));
        REQUIRE(v3i(2,2,0) >= v3i(2,1,0));
        REQUIRE(v3i(2,1,0) >= v3i(2,1,0));
        REQUIRE(v3i(1,2,0) >= v3i(1,2,0));

        REQUIRE_FALSE(v3i(2,1,0) >= v3i(3,2,0));
        REQUIRE_FALSE(v3i(2,1,0) >= v3i(2,2,0));
    }
    {
        REQUIRE((v3i(1,2,3)[0] == 1 && v3i(1,2,3)[1] == 2 && v3i(1,2,3)[2] == 3));
        REQUIRE((v3i(1,2,3).data()[0] == 1 && v3i(1,2,3).data()[1] == 2));
        REQUIRE((v3i(1,2,3).data()[2] == 3));
        v3i v = v3i::unit();
        v[0] = 3;
        v[1] = 4;
        v[2] = 5;
        REQUIRE(v == v3i{3,4,5});
    }
    {
        REQUIRE(-v3i(2,3,4) == v3i(-2,-3,-4));

        REQUIRE(2 + v3i(2,3,4) == v3i(4, 5, 6));
        REQUIRE(7 - v3i(4,5,6) == v3i(3, 2, 1));
        REQUIRE(2.f * v3f(2,3,4) == v3f(4, 6, 8));
        REQUIRE(8u / v3u(8,4,2) == v3u(1, 2, 4));

        REQUIRE(v3i(2,3,4) + 2 == v3i(4, 5, 6));
        REQUIRE(v3i(4,5,6) - 2 == v3i(2, 3, 4));
        REQUIRE(v3f(2,3,4) * 2.f == v3f(4, 6, 8));
        REQUIRE(v3u(8,6,4) / 2u == v3u(4, 3, 2));

        REQUIRE(v3i(2,3, 4) + v3i(2,3,4) == v3i(4, 6,  8));
        REQUIRE(v3i(4,5, 6) - v3i(2,4,3) == v3i(2, 1,  3));
        REQUIRE(v3f(2,3, 4) * v3f(4,3,3) == v3f(8, 9, 12));
        REQUIRE(v3u(8,9,12) / v3u(4,3,3) == v3u(2, 3,  4));
    }
    {
        REQUIRE(3.f + v3f(2) == v3f(5));
        REQUIRE(4.f - v3f(3) == v3f(1));
        REQUIRE(3.0 * v3d(2) == v3d(6));
        REQUIRE(6.0 / v3d(2) == v3d(3));

        REQUIRE(v3f(2) + 3.f == v3f(5));
        REQUIRE(v3f(3) - 2.f == v3f(1));
        REQUIRE(v3d(2) * 3.0 == v3d(6));
        REQUIRE(v3d(6) / 2.0 == v3d(3));

        REQUIRE(v3f(2) + v3f(3) == v3f(5));
        REQUIRE(v3f(3) - v3f(2) == v3f(1));
        REQUIRE(v3f(2) * v3f(3) == v3f(6));
        REQUIRE(v3f(6) / v3f(2) == v3f(3));
    }
    {
        v3i v = v3i::unit();
        REQUIRE((v += v3i(3,4,5)) == v3i(4, 5,  6));
        REQUIRE((v -= v3i(1,3,2)) == v3i(3, 2,  4));
        REQUIRE((v *= v3i(2,4,3)) == v3i(6, 8, 12));
        REQUIRE((v /= v3i(3,2,4)) == v3i(2, 4, 3));
    }
    {
        v3i v(1,2,3);
        REQUIRE((v += 2) == v3i(3, 4, 5));
        REQUIRE((v -= 1) == v3i(2, 3, 4));
        REQUIRE((v *= 2) == v3i(4, 6, 8));
        REQUIRE((v /= 2) == v3i(2, 3, 4));
    }
    {
        REQUIRE(math::approximately(v3i(0,1,2), v3i(1,2,3), 1));
        REQUIRE_FALSE(math::approximately(v3i(0,1,2), v3i(1,2,3)));
    }
    {
        REQUIRE(math::dot(v3i(2,3,4), v3i(4,5,6)) == 47);
        REQUIRE(math::dot(v3i(2,3,4), v3i(4,5,-6)) == -1);
        REQUIRE(math::abs_dot(v3i(2,3,4), v3i(4,5,-6)) == 47);
        REQUIRE(math::cross(v3i(2,3,4), v3i(4,5,6)) == v3i(-2,4,-2));

        REQUIRE(math::approximately(math::dot(v3f(2,3,4), v3f(4,5,6)), 47.f));
        REQUIRE(math::cross(v3f(2,3,4), v3f(4,5,6)) == v3f(-2,4,-2));
    }
    {
        REQUIRE(math::length_squared(v3i(0,0,0)) ==  0);
        REQUIRE(math::length_squared(v3i(2,0,0)) ==  4);
        REQUIRE(math::length_squared(v3i(0,3,0)) ==  9);
        REQUIRE(math::length_squared(v3u(0,0,4)) == 16);
        REQUIRE(math::length_squared(v3u(2,3,4)) == 29);

        REQUIRE(math::length(v3f(0,0,0)) == 0.f);
        REQUIRE(math::length(v3f(2,0,0)) == 2.f);
        REQUIRE(math::length(v3f(0,3,0)) == 3.f);
        REQUIRE(math::length(v3f(0,0,4)) == 4.f);
    }
    {
        REQUIRE(math::distance_squared(v3i(0,0,0), v3i(0,0,0)) ==  0);
        REQUIRE(math::distance_squared(v3i(4,0,0), v3i(0,0,0)) == 16);
        REQUIRE(math::distance_squared(v3i(8,0,0), v3i(2,0,0)) == 36);
        REQUIRE(math::distance_squared(v3i(0,0,0), v3i(0,4,0)) == 16);
        REQUIRE(math::distance_squared(v3i(0,2,0), v3i(0,8,0)) == 36);
        REQUIRE(math::distance_squared(v3u(0,0,0), v3u(0,0,4)) == 16);
        REQUIRE(math::distance_squared(v3u(0,0,2), v3u(0,0,8)) == 36);
        REQUIRE(math::distance_squared(v3u(0,0,4), v3u(0,0,0)) == 16);
        REQUIRE(math::distance_squared(v3u(0,0,8), v3u(0,0,2)) == 36);

        REQUIRE(math::distance(v3f(0,0,0), v3f(0,0,0)) == 0.f);
        REQUIRE(math::distance(v3f(4,0,0), v3f(0,0,0)) == 4.f);
        REQUIRE(math::distance(v3f(8,0,0), v3f(2,0,0)) == 6.f);
        REQUIRE(math::distance(v3f(0,0,0), v3f(0,4,0)) == 4.f);
        REQUIRE(math::distance(v3f(0,2,0), v3f(0,8,0)) == 6.f);
        REQUIRE(math::distance(v3f(0,0,0), v3f(0,0,4)) == 4.f);
        REQUIRE(math::distance(v3f(0,0,2), v3f(0,0,8)) == 6.f);
    }
    {
        v3f v0(2,0,0);
        v3f v1(0,2,0);
        v3f v2(0,0,2);
        REQUIRE(math::normalized(v0) == v3f(1,0,0));
        REQUIRE(math::normalized(v1) == v3f(0,1,0));
        REQUIRE(math::normalized(v2) == v3f(0,0,1));
        REQUIRE(v0 == v3f(2,0,0));
        REQUIRE(v1 == v3f(0,2,0));
        REQUIRE(v2 == v3f(0,0,2));

        REQUIRE(math::normalized(v3f(1,0,0)) == v3f(1,0,0));
        REQUIRE(math::normalized(v3f(0,1,0)) == v3f(0,1,0));
        REQUIRE(math::normalized(v3f(0,0,1)) == v3f(0,0,1));
    }
    {
        REQUIRE(math::minimum(v3i(1,2,3)) == 1);
        REQUIRE(math::minimum(v3i(2,1,3)) == 1);
        REQUIRE(math::minimum(v3i(3,2,1)) == 1);

        REQUIRE(math::maximum(v3i(1,2,3)) == 3);
        REQUIRE(math::maximum(v3i(2,1,3)) == 3);
        REQUIRE(math::maximum(v3i(3,2,1)) == 3);
    }
    {
        v3i v0(3,4,5);

        REQUIRE(math::minimized(v0, v3i(2)) == v3i(2));
        REQUIRE(math::minimized(v0, v3i(3)) == v3i(3));
        REQUIRE(math::minimized(v0, v3i(4)) == v3i(3,4,4));
        REQUIRE(math::minimized(v0, v3i(5)) == v3i(3,4,5));
        REQUIRE(math::minimized(v0, v3i(1,2,3)) == v3i(1,2,3));

        REQUIRE(math::maximized(v0, v3i(5)) == v3i(5));
        REQUIRE(math::maximized(v0, v3i(4)) == v3i(4,4,5));
        REQUIRE(math::maximized(v0, v3i(3)) == v3i(3,4,5));
        REQUIRE(math::maximized(v0, v3i(2)) == v3i(3,4,5));
        REQUIRE(math::maximized(v0, v3i(5,6,7)) == v3i(5,6,7));
    }
    {
        v3i v0(3,4,5);
        REQUIRE(math::clamped(v0, v3i(0,0,0), v3i(2,3,4)) == v3i(2,3,4));
        REQUIRE(v0 == v3i(3,4,5));
        REQUIRE(math::clamped(v0, v3i(3,4,5), v3i(3,4,5)) == v3i(3,4,5));
        REQUIRE(v0 == v3i(3,4,5));

        REQUIRE(math::clamped(v0, v3i(0,0,0), v3i(0,0,0)) == v3i(0,0,0));
        REQUIRE(math::clamped(v0, v3i(0,0,0), v3i(3,2,1)) == v3i(3,2,1));
        REQUIRE(math::clamped(v0, v3i(0,0,0), v3i(4,3,2)) == v3i(3,3,2));

        REQUIRE(math::clamped(v0, v3i(4,5,6), v3i(9,9,9)) == v3i(4,5,6));
        REQUIRE(math::clamped(v0, v3i(6,5,4), v3i(9,9,9)) == v3i(6,5,5));
        REQUIRE(math::clamped(v0, v3i(7,6,5), v3i(9,9,9)) == v3i(7,6,5));

        REQUIRE(math::saturated(v3i(-1,-2,-3)) == v3i(0,0,0));
        REQUIRE(math::saturated(v3i( 2, 3, 4)) == v3i(1,1,1));
        REQUIRE(math::saturated(v3i(-1, 3, 4)) == v3i(0,1,1));

        REQUIRE(math::saturated(v3f(2,0.6f,2)) == v3f(1,0.6f,1));
        REQUIRE(math::saturated(v3f(0.6f,-2,2)) == v3f(0.6f,0,1));
    }
    {
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), 0.f) == v3f(1,2,1));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), 0.5f) == v3f(5.5f,11,5.5f));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), 1.f) == v3f(10,20,10));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), 2.f) == v3f(19,38,19));

        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(0.f)) == v3f(1,2,1));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(0.5f)) == v3f(5.5f,11,5.5f));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(1.f)) == v3f(10,20,10));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(2.f)) == v3f(19,38,19));

        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(0.f,1.f,0.f)) == v3f(1,20,1));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(0.5f,2.f,0.5f)) == v3f(5.5f,38,5.5f));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(1.f,0.5f,1.f)) == v3f(10,11,10));
        REQUIRE(math::lerp(v3f(1,2,1), v3f(10,20,10), v3f(2.f,0.f,2.f)) == v3f(19,2,19));

        REQUIRE(math::inverse_lerp(v3f(1,2,1), v3f(10,20,10), v3f(5.5f,11,5.5f)) == v3f(0.5f));
        REQUIRE(math::inverse_lerp(v3f(1,2,1), v3f(10,20,10), v3f(5.5f,38,5.5f)) == v3f(0.5f,2.f,0.5f));
    }
    {
        REQUIRE_FALSE(math::contains_nan(v3i(0,1,2)));
        REQUIRE_FALSE(math::contains_nan(v3f(0.f,1.f,2.f)));
        REQUIRE(math::contains_nan(v3f(0.f,1.f,std::numeric_limits<f32>::quiet_NaN())));
        REQUIRE(math::contains_nan(v3f(std::numeric_limits<f32>::infinity(),1.f,2.f)));
        REQUIRE(math::contains_nan(v3f(1.f,std::numeric_limits<f32>::infinity(),2.f)));
    }
}
