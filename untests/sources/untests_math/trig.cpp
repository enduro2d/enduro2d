/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("trig") {
    {
        auto r2_f32 = math::pi<f32>();
        static_assert(std::is_same<f32, decltype(r2_f32)::value_type>::value, "");
        auto r3_f64 = math::inv_pi<f64>();
        static_assert(std::is_same<f64, decltype(r3_f64)::value_type>::value, "");
    }
    {
        auto d_u8 = make_deg(u8(0));
        auto d_u16 = make_deg(u16(0));
        auto d_f32 = make_deg(f32(0));
        auto d_f64 = make_deg(f64(0));
        static_assert(std::is_same<u8, decltype(d_u8)::value_type>::value, "");
        static_assert(std::is_same<u16, decltype(d_u16)::value_type>::value, "");
        static_assert(std::is_same<f32, decltype(d_f32)::value_type>::value, "");
        static_assert(std::is_same<f64, decltype(d_f64)::value_type>::value, "");

        auto r_u8 = math::to_rad(d_u8);
        auto r_u16 = math::to_rad(d_u16);
        auto r_f32 = math::to_rad(d_f32);
        auto r_f64 = math::to_rad(d_f64);
        static_assert(std::is_same<u8, decltype(r_u8)::value_type>::value, "");
        static_assert(std::is_same<u16, decltype(r_u16)::value_type>::value, "");
        static_assert(std::is_same<f32, decltype(r_f32)::value_type>::value, "");
        static_assert(std::is_same<f64, decltype(r_f64)::value_type>::value, "");
    }
    {
        REQUIRE(make_rad(1.f) == rad<f32>(1.f));
        REQUIRE_FALSE(make_rad(1.f) == rad<f32>(2.f));

        REQUIRE(rad<f32>(rad<f32>(1.f)) == rad<f32>(1.f));
        REQUIRE_FALSE(rad<f32>(rad<f32>(1.f)) == rad<f32>(2.f));

        auto r0 = make_rad(1.f);
        r0 = rad<f32>(2.f);
        REQUIRE(r0 == make_rad(2.f));
        r0 *= 2;
        REQUIRE(r0 == make_rad(4.f));
        r0 /= 2;
        REQUIRE(r0 == make_rad(2.f));
        r0 += rad<f32>(3.f);
        REQUIRE(r0 == make_rad(5.f));
        r0 -= rad<f32>(2.f);
        REQUIRE(r0 == make_rad(3.f));

        REQUIRE(deg<f32>(math::half_pi<f32>()) == deg<f32>(90.f));

        REQUIRE(deg<f32>(90).cast_to<i32>() == make_deg(90));
        REQUIRE(math::pi<f32>().convert_to<deg_tag>() == make_deg(180.f));
    }
    {
        REQUIRE(math::approximately(math::pi<f32>(), make_rad(3.14f), 0.01f));
        REQUIRE(math::approximately(math::inv_pi<f32>(), 1.f / make_rad(3.14f), 0.01f));
        REQUIRE(math::approximately(math::two_pi<f32>(), 2.f * make_rad(3.14f), 0.01f));
        REQUIRE(math::approximately(math::four_pi<f32>(), 4.f * make_rad(3.14f), 0.01f));
        REQUIRE(math::approximately(math::half_pi<f32>(), 0.5f * make_rad(3.14f), 0.01f));
        REQUIRE(math::approximately(math::quarter_pi<f32>(), 0.25f * make_rad(3.14f), 0.01f));
    }
    {
        REQUIRE(make_deg(180.f) == make_deg(180.f));
        REQUIRE(make_rad(3.14159f) == make_rad(3.14159f));

        REQUIRE_FALSE(make_deg(180.f) == make_deg(181.f));
        REQUIRE_FALSE(make_rad(3.14159f) == make_rad(3.15f));

        REQUIRE(make_deg(180.f) < make_deg(181.f));
        REQUIRE(make_deg(181.f) > make_deg(180.f));
        REQUIRE_FALSE(make_deg(181.f) < make_deg(180.f));
        REQUIRE_FALSE(make_deg(180.f) > make_deg(181.f));

        REQUIRE(make_deg(180.f) <= make_deg(181.f));
        REQUIRE(make_deg(180.f) <= make_deg(180.f));
        REQUIRE_FALSE(make_deg(181.f) <= make_deg(180.f));

        REQUIRE(make_deg(181.f) >= make_deg(180.f));
        REQUIRE(make_deg(180.f) >= make_deg(180.f));
        REQUIRE_FALSE(make_deg(180.f) >= make_deg(181.f));

        REQUIRE(math::approximately(math::cos(make_deg(0.f)), 1.f));
    }
    {
        REQUIRE(-make_deg(180.f) == deg<f32>(-180));
        REQUIRE(math::approximately(make_deg(180.f), make_deg(180.f)));
        REQUIRE(math::approximately(make_deg(180.f), make_deg(181.f), 1.5f));
        REQUIRE_FALSE(math::approximately(make_deg(180.f), make_deg(181.f)));
        REQUIRE_FALSE(math::approximately(make_deg(180.f), make_deg(182.f), 1.f));
    }
    {
        REQUIRE(make_deg(90.f) * 2.f == make_deg(180.f));
        REQUIRE(2.f * make_deg(90.f) == make_deg(180.f));
        REQUIRE(make_deg(90.f) / 2.f == make_deg(45.f));
        REQUIRE(180.f / make_deg(90.f) == make_deg(2.f));

        REQUIRE(make_deg(90.f) + make_deg(180.f) == make_deg(270.f));
        REQUIRE(make_deg(90.0) + make_deg(180.0) == make_deg(270.0));

        REQUIRE(make_deg(270.f) - make_deg(180.f) == make_deg(90.f));
        REQUIRE(make_deg(270.0) - make_deg(180.0) == make_deg(90.0));
    }
    {
        auto a0 = make_deg(90.f);
        REQUIRE(&a0 == &(a0 *= 2.f));
        REQUIRE(a0 == make_deg(180.f));
        REQUIRE(&a0 == &(a0 /= 2.f));
        REQUIRE(a0 == make_deg(90.f));

        auto a1 = make_deg(90.f);
        REQUIRE(&a1 == &(a1 += make_deg(90.f)));
        REQUIRE(a1 == make_deg(180.f));
        REQUIRE(&a1 == &(a1 -= make_deg(90.f)));
        REQUIRE(a1 == make_deg(90.f));
    }
    {
        REQUIRE(math::minimized(make_deg(180.f), make_deg(170.f)) == make_deg(170.f));
        REQUIRE(math::minimized(make_deg(170.f), make_deg(180.f)) == make_deg(170.f));

        REQUIRE(math::maximized(make_deg(180.f), make_deg(170.f)) == make_deg(180.f));
        REQUIRE(math::maximized(make_deg(170.f), make_deg(180.f)) == make_deg(180.f));

        REQUIRE(math::clamped(make_deg(180.f), make_deg(170.f), make_deg(190.f)) == make_deg(180.f));
        REQUIRE(math::clamped(make_deg(160.f), make_deg(170.f), make_deg(190.f)) == make_deg(170.f));
        REQUIRE(math::clamped(make_deg(200.f), make_deg(170.f), make_deg(190.f)) == make_deg(190.f));

        REQUIRE(math::saturated(make_rad(-1.f)) == make_rad(0.f));
        REQUIRE(math::saturated(make_rad(0.5f)) == make_rad(0.5f));
        REQUIRE(math::saturated(make_rad(1.5f)) == make_rad(1.f));
    }
    {
        REQUIRE(math::to_rad(rad<f32>(math::pi<f32>())) == make_rad(3.14159f));
        REQUIRE(math::to_rad(deg<f32>(180.f)) == make_rad(3.14159f));
        REQUIRE(math::approximately(math::to_rad(make_deg(180.f)), make_rad(3.f), 0.2f));
        REQUIRE(math::approximately(math::to_rad(make_deg(360.0)), make_rad(6.0), 0.4));

        REQUIRE(math::to_deg(rad<f32>(math::pi<f32>())) == make_deg(180.f));
        REQUIRE(math::to_deg(deg<f32>(180.f)) == make_deg(180.f));
        REQUIRE(math::approximately(math::to_deg(make_rad(3.f)), make_deg(180.f), 10.f));
        REQUIRE(math::approximately(math::to_deg(make_rad(6.0)), make_deg(360.0), 20.0));
    }
    {
        using namespace e2d::math;
        REQUIRE(math::approximately(math::cos(make_deg(45.f)), std::cos(quarter_pi<f32>().value)));
        REQUIRE(math::approximately(math::sin(make_deg(45.f)), std::sin(quarter_pi<f32>().value)));
        REQUIRE(math::approximately(math::tan(make_deg(45.f)), std::tan(quarter_pi<f32>().value)));
        REQUIRE(math::acos(make_deg(45.f)) == make_rad(std::acos(quarter_pi<f32>().value)));
        REQUIRE(math::asin(make_deg(45.f)) == make_rad(std::asin(quarter_pi<f32>().value)));
        REQUIRE(math::atan(make_deg(45.f)) == make_rad(std::atan(quarter_pi<f32>().value)));
        REQUIRE(math::atan2(10.f, 20.f) == make_rad(std::atan2(10.f, 20.f)));
    }
    {
        using namespace e2d::math;

        REQUIRE(normalized_angle(make_rad(0.f)) == make_rad(0.f));

        REQUIRE(normalized_angle(half_pi<f32>()) == half_pi<f32>());
        REQUIRE(normalized_angle(-half_pi<f32>()) == -half_pi<f32>());

        REQUIRE(normalized_angle(pi<f32>()) == pi<f32>());
        REQUIRE(normalized_angle(-pi<f32>()) == -pi<f32>());

        REQUIRE(normalized_angle(two_pi<f32>()) == -two_pi<f32>());
        REQUIRE(normalized_angle(-two_pi<f32>()) == -two_pi<f32>());

        REQUIRE(normalized_angle(2.f * two_pi<f32>()) == make_rad(0.f));
        REQUIRE(normalized_angle(-2.f * two_pi<f32>()) == make_rad(0.f));

        REQUIRE(normalized_angle(two_pi<f32>() + pi<f32>()) == -pi<f32>());
        REQUIRE(normalized_angle(-two_pi<f32>() - pi<f32>()) == pi<f32>());
    }
}
