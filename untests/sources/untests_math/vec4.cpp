/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("vec4") {
    {
        REQUIRE(v4i::zero() == v4i(0));
        REQUIRE(v4i::unit() == v4i(1));
        REQUIRE(v4i::unit_x() == v4i(1,0,0,0));
        REQUIRE(v4i::unit_y() == v4i(0,1,0,0));
        REQUIRE(v4i::unit_z() == v4i(0,0,1,0));
        REQUIRE(v4i::unit_w() == v4i(0,0,0,1));
    }
    {
        static_assert(std::is_same<
            vec4<i32>, v4i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, v4i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v4i>().x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v4i>().y)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v4i>().z)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<v4i>().w)
        >::value, "static unit test error");

        static_assert(std::is_same<
            vec4<f32>, v4f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, v4f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v4f>().x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v4f>().y)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v4f>().z)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<v4f>().w)
        >::value, "static unit test error");
    }
    {
        auto v0 = v4i();
        auto v1 = v4i(1);
        auto v2 = v4i(1,2,3,4);
        auto v3 = v4i(v2i(3,4),5,6);
        auto v4 = v4i(v3i(3,4,5),6);
        REQUIRE((v0.x == 0 && v0.y == 0 && v0.z == 0 && v0.w == 0));
        REQUIRE((v1.x == 1 && v1.y == 1 && v1.z == 1 && v1.w == 1));
        REQUIRE((v2.x == 1 && v2.y == 2 && v2.z == 3 && v2.w == 4));
        REQUIRE((v3.x == 3 && v3.y == 4 && v3.z == 5 && v3.w == 6));
        REQUIRE((v4.x == 3 && v4.y == 4 && v4.z == 5 && v4.w == 6));
    }
    {
        REQUIRE(make_vec4(1) == v4i(1,1,1,1));
        REQUIRE(make_vec4(1,2,3,4) == v4i(1,2,3,4));
        REQUIRE(make_vec4(v2i(1,2),3,4) == v4i(1,2,3,4));
        REQUIRE(make_vec4(v3i(1,2,3),4) == v4i(1,2,3,4));
    }
    {
        auto v0 = v4i(1,2,3,4);
        auto v1 = v4i(v0);
        REQUIRE((v0.x == 1 && v0.y == 2 && v0.z == 3 && v0.w == 4));
        REQUIRE((v1.x == 1 && v1.y == 2 && v1.z == 3 && v1.w == 4));
        v1 = v4i(2,3,4,5);
        REQUIRE((v1.x == 2 && v1.y == 3 && v1.z == 4 && v1.w == 5));
    }
    {
        REQUIRE(v4i(1,2,3,4) == v4i(1,2,3,4));
        REQUIRE_FALSE(v4i(1,2,3,4) == v4i(2,1,1,4));
        REQUIRE_FALSE(v4i(1,2,3,4) == v4i(1,1,1,4));

        REQUIRE_FALSE(v4i(1,2,3,4) != v4i(1,2,3,4));
        REQUIRE(v4i(1,2,3,4) != v4i(2,1,1,4));
        REQUIRE(v4i(1,2,3,4) != v4i(1,1,1,4));
    }
    {
        REQUIRE(v4i(2,1,0,0) < v4i(3,2,0,0));
        REQUIRE(v4i(2,1,0,0) < v4i(2,2,0,0));
        REQUIRE(v4i(2,1,0,0) < v4i(2,1,1,0));

        REQUIRE_FALSE(v4i(3,2,0,0) < v4i(2,1,0,0));
        REQUIRE_FALSE(v4i(2,2,0,0) < v4i(2,1,0,0));
        REQUIRE_FALSE(v4i(2,2,1,0) < v4i(2,2,0,0));

        REQUIRE(v4i(2,1,0,0) <= v4i(3,2,0,0));
        REQUIRE(v4i(2,1,0,0) <= v4i(2,2,0,0));
        REQUIRE(v4i(2,1,0,0) <= v4i(2,1,0,0));
        REQUIRE(v4i(1,2,0,0) <= v4i(1,2,0,0));

        REQUIRE_FALSE(v4i(3,2,0,0) <= v4i(2,1,0,0));
        REQUIRE_FALSE(v4i(2,2,0,0) <= v4i(2,1,0,0));

        REQUIRE(v4i(3,2,0,0) >= v4i(2,1,0,0));
        REQUIRE(v4i(2,2,0,0) >= v4i(2,1,0,0));
        REQUIRE(v4i(2,1,0,0) >= v4i(2,1,0,0));
        REQUIRE(v4i(1,2,0,0) >= v4i(1,2,0,0));

        REQUIRE_FALSE(v4i(2,1,0,0) >= v4i(3,2,0,0));
        REQUIRE_FALSE(v4i(2,1,0,0) >= v4i(2,2,0,0));
    }
    {
        REQUIRE((v4i(1,2,3,4)[0] == 1 && v4i(1,2,3,4)[1] == 2));
        REQUIRE((v4i(1,2,3,4)[2] == 3 && v4i(1,2,3,4)[3] == 4));
        REQUIRE((v4i(1,2,3,4).data()[0] == 1 && v4i(1,2,3,4).data()[1] == 2));
        REQUIRE((v4i(1,2,3,4).data()[2] == 3 && v4i(1,2,3,4).data()[3] == 4));
        v4i v = v4i::unit();
        v[0] = 3;
        v[1] = 4;
        v[2] = 5;
        v[3] = 6;
        REQUIRE(v == v4i{3,4,5,6});
    }
    {
        REQUIRE(-v4i(2,3,4,5) == v4i(-2,-3,-4,-5));

        REQUIRE(2 + v4i(2,3,4,5) == v4i(4, 5, 6, 7));
        REQUIRE(7 - v4i(4,5,6,7) == v4i(3, 2, 1, 0));
        REQUIRE(2.f * v4f(2,3,4,5) == v4f(4, 6, 8, 10));
        REQUIRE(8u / v4u(8,4,2,1) == v4u(1, 2, 4, 8));

        REQUIRE(v4i(2,3,4,5) + 2 == v4i(4, 5, 6, 7));
        REQUIRE(v4i(4,5,6,7) - 2 == v4i(2, 3, 4, 5));
        REQUIRE(v4f(2,3,4,5) * 2.f == v4f(4, 6, 8, 10));
        REQUIRE(v4u(8,6,4,2) / 2u == v4u(4, 3, 2, 1));

        REQUIRE(v4i(2,3, 4,5) + v4i(2,3,4,5) == v4i(4, 6,  8, 10));
        REQUIRE(v4i(4,5, 6,7) - v4i(2,4,3,1) == v4i(2, 1,  3, 6));
        REQUIRE(v4f(2,3, 4,5) * v4f(4,3,3,2) == v4f(8, 9, 12, 10));
        REQUIRE(v4u(8,9,12,14) / v4u(4,3,3,2) == v4u(2, 3,  4, 7));
    }
    {
        REQUIRE(3.f + v4f(2) == v4f(5));
        REQUIRE(4.f - v4f(3) == v4f(1));
        REQUIRE(3.0 * v4d(2) == v4d(6));
        REQUIRE(6.0 / v4d(2) == v4d(3));

        REQUIRE(v4f(2) + 3.f == v4f(5));
        REQUIRE(v4f(3) - 2.f == v4f(1));
        REQUIRE(v4d(2) * 3.0 == v4d(6));
        REQUIRE(v4d(6) / 2.0 == v4d(3));

        REQUIRE(v4f(2) + v4f(3) == v4f(5));
        REQUIRE(v4f(3) - v4f(2) == v4f(1));
        REQUIRE(v4f(2) * v4f(3) == v4f(6));
        REQUIRE(v4f(6) / v4f(2) == v4f(3));
    }
    {
        v4i v = v4i::unit();
        REQUIRE((v += v4i(3,4,5,6)) == v4i(4, 5, 6, 7));
        REQUIRE((v -= v4i(1,3,2,4)) == v4i(3, 2,  4, 3));
        REQUIRE((v *= v4i(2,4,3,2)) == v4i(6, 8, 12, 6));
        REQUIRE((v /= v4i(3,2,4,3)) == v4i(2, 4, 3, 2));
    }
    {
        v4i v(1,2,3,4);
        REQUIRE((v += 2) == v4i(3, 4, 5, 6));
        REQUIRE((v -= 1) == v4i(2, 3, 4, 5));
        REQUIRE((v *= 2) == v4i(4, 6, 8, 10));
        REQUIRE((v /= 2) == v4i(2, 3, 4, 5));
    }
    {
        REQUIRE(math::approximately(v4i(0,1,2,3), v4i(1,2,3,4), 1));
        REQUIRE_FALSE(math::approximately(v4i(0,1,2,3), v4i(1,2,3,4)));
    }
    {
        REQUIRE(math::dot(v4i(2,3,4,5), v4i(4,5,6,7)) == 82);
        REQUIRE(math::dot(v4i(2,3,4,5), v4i(4,5,6,-7)) == 12);
        REQUIRE(math::abs_dot(v4i(2,3,4,5), v4i(4,5,6,-7)) == 82);
        REQUIRE(math::approximately(math::dot(v4f(2,3,4,5), v4f(4,5,6,7)), 82.f));
    }
    {
        REQUIRE(math::length_squared(v4i(0,0,0,0)) ==  0);
        REQUIRE(math::length_squared(v4i(2,0,0,0)) ==  4);
        REQUIRE(math::length_squared(v4i(0,3,0,0)) ==  9);
        REQUIRE(math::length_squared(v4u(0,0,4,0)) == 16);
        REQUIRE(math::length_squared(v4u(0,0,0,5)) == 25);
        REQUIRE(math::length_squared(v4u(2,3,4,5)) == 54);

        REQUIRE(math::length(v4f(0,0,0,0)) == 0.f);
        REQUIRE(math::length(v4f(2,0,0,0)) == 2.f);
        REQUIRE(math::length(v4f(0,3,0,0)) == 3.f);
        REQUIRE(math::length(v4f(0,0,4,0)) == 4.f);
        REQUIRE(math::length(v4f(0,0,0,5)) == 5.f);
    }
    {
        REQUIRE(math::distance_squared(v4i(0,0,0,0), v4i(0,0,0,0)) ==  0);
        REQUIRE(math::distance_squared(v4i(4,0,0,0), v4i(0,0,0,0)) == 16);
        REQUIRE(math::distance_squared(v4i(8,0,0,0), v4i(2,0,0,0)) == 36);
        REQUIRE(math::distance_squared(v4i(0,0,0,0), v4i(0,4,0,0)) == 16);
        REQUIRE(math::distance_squared(v4i(0,2,0,0), v4i(0,8,0,0)) == 36);
        REQUIRE(math::distance_squared(v4u(0,0,0,0), v4u(0,0,4,0)) == 16);
        REQUIRE(math::distance_squared(v4u(0,0,2,0), v4u(0,0,8,0)) == 36);
        REQUIRE(math::distance_squared(v4u(0,0,4,0), v4u(0,0,0,0)) == 16);
        REQUIRE(math::distance_squared(v4u(0,0,8,0), v4u(0,0,2,0)) == 36);
        REQUIRE(math::distance_squared(v4u(0,0,0,2), v4u(0,0,0,8)) == 36);
        REQUIRE(math::distance_squared(v4u(0,0,0,4), v4u(0,0,0,0)) == 16);
        REQUIRE(math::distance_squared(v4u(0,0,0,8), v4u(0,0,0,2)) == 36);

        REQUIRE(math::distance(v4f(0,0,0,0), v4f(0,0,0,0)) == 0.f);
        REQUIRE(math::distance(v4f(4,0,0,0), v4f(0,0,0,0)) == 4.f);
        REQUIRE(math::distance(v4f(8,0,0,0), v4f(2,0,0,0)) == 6.f);
        REQUIRE(math::distance(v4f(0,0,0,0), v4f(0,4,0,0)) == 4.f);
        REQUIRE(math::distance(v4f(0,2,0,0), v4f(0,8,0,0)) == 6.f);
        REQUIRE(math::distance(v4f(0,0,0,0), v4f(0,0,4,0)) == 4.f);
        REQUIRE(math::distance(v4f(0,0,2,0), v4f(0,0,8,0)) == 6.f);
        REQUIRE(math::distance(v4f(0,0,0,0), v4f(0,0,0,4)) == 4.f);
        REQUIRE(math::distance(v4f(0,0,0,2), v4f(0,0,0,8)) == 6.f);
    }
    {
        v4f v0(2,0,0,0);
        v4f v1(0,2,0,0);
        v4f v2(0,0,2,0);
        v4f v3(0,0,0,2);
        REQUIRE(math::normalized(v0) == v4f(1,0,0,0));
        REQUIRE(math::normalized(v1) == v4f(0,1,0,0));
        REQUIRE(math::normalized(v2) == v4f(0,0,1,0));
        REQUIRE(math::normalized(v3) == v4f(0,0,0,1));
        REQUIRE(v0 == v4f(2,0,0,0));
        REQUIRE(v1 == v4f(0,2,0,0));
        REQUIRE(v2 == v4f(0,0,2,0));
        REQUIRE(v3 == v4f(0,0,0,2));

        REQUIRE(math::normalized(v4f(1,0,0,0)) == v4f(1,0,0,0));
        REQUIRE(math::normalized(v4f(0,1,0,0)) == v4f(0,1,0,0));
        REQUIRE(math::normalized(v4f(0,0,1,0)) == v4f(0,0,1,0));
        REQUIRE(math::normalized(v4f(0,0,0,1)) == v4f(0,0,0,1));
    }
    {
        REQUIRE(math::minimum(v4i(1,2,3,4)) == 1);
        REQUIRE(math::minimum(v4i(2,1,3,4)) == 1);
        REQUIRE(math::minimum(v4i(4,3,2,1)) == 1);

        REQUIRE(math::maximum(v4i(1,2,3,4)) == 4);
        REQUIRE(math::maximum(v4i(2,1,3,4)) == 4);
        REQUIRE(math::maximum(v4i(4,3,2,1)) == 4);
    }
    {
        v4i v0(3,4,5,6);

        REQUIRE(math::minimized(v0, v4i(2)) == v4i(2));
        REQUIRE(math::minimized(v0, v4i(3)) == v4i(3));
        REQUIRE(math::minimized(v0, v4i(4)) == v4i(3,4,4,4));
        REQUIRE(math::minimized(v0, v4i(5)) == v4i(3,4,5,5));
        REQUIRE(math::minimized(v0, v4i(1,2,3,4)) == v4i(1,2,3,4));

        REQUIRE(math::maximized(v0, v4i(5)) == v4i(5,5,5,6));
        REQUIRE(math::maximized(v0, v4i(4)) == v4i(4,4,5,6));
        REQUIRE(math::maximized(v0, v4i(3)) == v4i(3,4,5,6));
        REQUIRE(math::maximized(v0, v4i(2)) == v4i(3,4,5,6));
        REQUIRE(math::maximized(v0, v4i(5,6,7,8)) == v4i(5,6,7,8));
    }
    {
        v4i v0(3,4,5,6);
        REQUIRE(math::clamped(v0, v4i(0,0,0,0), v4i(2,3,4,5)) == v4i(2,3,4,5));
        REQUIRE(v0 == v4i(3,4,5,6));
        REQUIRE(math::clamped(v0, v4i(3,4,5,6), v4i(3,4,5,6)) == v4i(3,4,5,6));
        REQUIRE(v0 == v4i(3,4,5,6));

        REQUIRE(math::clamped(v0, v4i(0,0,0,0), v4i(0,0,0,0)) == v4i(0,0,0,0));
        REQUIRE(math::clamped(v0, v4i(0,0,0,0), v4i(3,2,1,0)) == v4i(3,2,1,0));
        REQUIRE(math::clamped(v0, v4i(0,0,0,0), v4i(4,3,2,1)) == v4i(3,3,2,1));

        REQUIRE(math::clamped(v0, v4i(4,5,6,7), v4i(9,9,9,9)) == v4i(4,5,6,7));
        REQUIRE(math::clamped(v0, v4i(6,5,4,3), v4i(9,9,9,9)) == v4i(6,5,5,6));
        REQUIRE(math::clamped(v0, v4i(7,6,5,4), v4i(9,9,9,9)) == v4i(7,6,5,6));

        REQUIRE(math::saturated(v4i(-1,-2,-3,-4)) == v4i(0,0,0,0));
        REQUIRE(math::saturated(v4i( 2, 3, 4, 5)) == v4i(1,1,1,1));
        REQUIRE(math::saturated(v4i(-1, 3, 4, 5)) == v4i(0,1,1,1));

        REQUIRE(math::saturated(v4f(2,0.6f,2,0.7f)) == v4f(1,0.6f,1,0.7f));
        REQUIRE(math::saturated(v4f(0.6f,-2,2,2)) == v4f(0.6f,0,1,1));
    }
    {
        REQUIRE_FALSE(math::contains_nan(v4i(0,1,2,3)));
        REQUIRE_FALSE(math::contains_nan(v4f(0.f,1.f,2.f,3.f)));
        REQUIRE(math::contains_nan(v4f(0.f,1.f,2.f,std::numeric_limits<f32>::quiet_NaN())));
        REQUIRE(math::contains_nan(v4f(0.f,1.f,std::numeric_limits<f32>::quiet_NaN(),2.f)));
        REQUIRE(math::contains_nan(v4f(std::numeric_limits<f32>::infinity(),1.f,2.f,3.f)));
        REQUIRE(math::contains_nan(v4f(1.f,std::numeric_limits<f32>::infinity(),2.f,3.f)));
    }
}
