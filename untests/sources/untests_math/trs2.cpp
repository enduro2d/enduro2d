/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("trs2") {
    {
        REQUIRE(t2i::zero().translation == v2i(0));
        REQUIRE(t2i::zero().rotation == 0);
        REQUIRE(t2i::zero().scale == v2i(0));

        REQUIRE(t2i::identity().translation == v2i(0));
        REQUIRE(t2i::identity().rotation == 0);
        REQUIRE(t2i::identity().scale == v2i(1));
    }
    {
        static_assert(std::is_same<
            trs2<i32>, t2i::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, t2i::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<t2i>().translation.x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<t2i>().rotation)
        >::value, "static unit test error");
        static_assert(std::is_same<
            i32, decltype(std::declval<t2i>().scale.x)
        >::value, "static unit test error");

        static_assert(std::is_same<
            trs2<f32>, t2f::self_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, t2f::value_type
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<t2f>().translation.x)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<t2f>().rotation)
        >::value, "static unit test error");
        static_assert(std::is_same<
            f32, decltype(std::declval<t2f>().scale.x)
        >::value, "static unit test error");
    }
    {
        auto t0 = t2f();
        auto t1 = t2f(v2f(1.f,2.f), 3.f, v2f(4.f,5.f));
        auto t2 = t1;

        REQUIRE(t0.translation == v2f(0.f));
        REQUIRE(math::approximately(t0.rotation, 0.f));
        REQUIRE(t0.scale == v2f(1.f));

        REQUIRE(t1.translation == v2f(1.f,2.f));
        REQUIRE(math::approximately(t1.rotation, 3.f));
        REQUIRE(t1.scale == v2f(4.f,5.f));

        REQUIRE(t2.translation == v2f(1.f,2.f));
        REQUIRE(math::approximately(t2.rotation, 3.f));
        REQUIRE(t2.scale == v2f(4.f,5.f));

        REQUIRE(t0 == t2f::identity());
        REQUIRE(t1 == t2);
        REQUIRE_FALSE(t0 == t1);
        REQUIRE_FALSE(t1 != t2);
        REQUIRE_FALSE(t0 != t2f::identity());
        REQUIRE(t0 != t1);
    }
    {
        auto t1 = make_trs2(v2f(1.f,2.f), 3.f, v2f(4.f,5.f));
        auto t2 = t1.cast_to<i32>();
        REQUIRE(t2.translation == v2i(1,2));
        REQUIRE(t2.rotation == 3);
        REQUIRE(t2.scale == v2i(4,5));
    }
    {
        auto t1 = math::make_translation_trs2(v2f(3.f,4.f));
        REQUIRE(t1.translation == v2f(3.f,4.f));
        REQUIRE(math::approximately(t1.rotation, 0.f));
        REQUIRE(t1.scale == v2f::unit());
    }
    {
        auto t1 = math::make_rotation_trs2(3.f);
        REQUIRE(t1.translation == v2f::zero());
        REQUIRE(math::approximately(t1.rotation, 3.f));
        REQUIRE(t1.scale == v2f::unit());
    }
    {
        auto t1 = math::make_scale_trs2(v2f(3.f,4.f));
        REQUIRE(t1.translation == v2f::zero());
        REQUIRE(math::approximately(t1.rotation, 0.f));
        REQUIRE(t1.scale == make_vec2(3.f,4.f));
    }
    {
        REQUIRE(math::approximately(
            math::make_translation_trs2(v2i{1,2}),
            math::make_translation_trs2(v2i{1,4}),
            2));
        REQUIRE_FALSE(math::approximately(
            math::make_translation_trs2(v2i{1,2}),
            math::make_translation_trs2(v2i{1,4}),
            1));
    }
}
