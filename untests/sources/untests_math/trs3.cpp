/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("trs3") {
    {
        REQUIRE(t3i::zero().translation == v3i(0));
        REQUIRE(t3i::zero().rotation == v3i::zero());
        REQUIRE(t3i::zero().scale == v3i(0));

        REQUIRE(t3i::identity().translation == v3i(0));
        REQUIRE(t3i::identity().rotation == v3i(0));
        REQUIRE(t3i::identity().scale == v3i(1));
    }
    {
        static_assert(std::is_same<
            trs3<i32>, t3i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, t3i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<t3i>().translation.x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<t3i>().rotation.x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<t3i>().scale.x)
        >::value, "static unit test error");

        static_assert(std::is_same<
            trs3<f32>, t3f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, t3f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<t3f>().translation.x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<t3f>().rotation.x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<t3f>().scale.x)
        >::value, "static unit test error");
    }
    {
        auto t0 = t3f();
        auto t1 = t3f(
            v3f(1.f,2.f,3.f),
            v3f(2.f,3.f,4.f),
            v3f(4.f,5.f,6.f));

        REQUIRE(t0.translation == v3f(0.f));
        REQUIRE(t0.rotation == v3f(0.f));
        REQUIRE(t0.scale == v3f(1.f));

        REQUIRE(t1.translation == v3f(1.f,2.f,3.f));
        REQUIRE(t1.rotation == v3f(2.f,3.f,4.f));
        REQUIRE(t1.scale == v3f(4.f,5.f,6.f));

        REQUIRE(t0 == t3f::identity());
        REQUIRE_FALSE(t0 == t1);
        REQUIRE_FALSE(t0 != t3f::identity());
        REQUIRE(t0 != t1);
    }
    {
        auto t1 = make_trs3(
            v3f(1.f,2.f,3.f),
            v3f(2.f,3.f,4.f),
            v3f(4.f,5.f,6.f));
        auto t2 = t1.cast_to<i32>();
        REQUIRE(t2.translation == v3i(1,2,3));
        REQUIRE(t2.rotation == v3i(2,3,4));
        REQUIRE(t2.scale == v3i(4,5,6));
    }
    {
        auto t1 = math::make_translation_trs3(v3f(3.f,4.f,5.f));
        REQUIRE(t1.translation == v3f(3.f,4.f,5.f));
        REQUIRE(t1.rotation == v3f::zero());
        REQUIRE(t1.scale == v3f::unit());
    }
    {
        auto t1 = math::make_rotation_trs3(make_vec3(2,3,4));
        REQUIRE(t1.translation == v3i::zero());
        REQUIRE(t1.rotation == make_vec3(2,3,4));
        REQUIRE(t1.scale == v3i::unit());
    }
    {
        auto t1 = math::make_scale_trs3(v3f(3.f,4.f,5.f));
        REQUIRE(t1.translation == v3f::zero());
        REQUIRE(t1.rotation == v3f::zero());
        REQUIRE(t1.scale == make_vec3(3.f,4.f,5.f));
    }
    {
        REQUIRE(math::approximately(
            math::make_translation_trs3(v3i{1,2,5}),
            math::make_translation_trs3(v3i{1,4,5}),
            2));
        REQUIRE_FALSE(math::approximately(
            math::make_translation_trs3(v3i{1,2,5}),
            math::make_translation_trs3(v3i{1,4,5}),
            1));
    }
}
