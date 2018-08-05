/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#define CATCH_CONFIG_MAIN
#include "_math.hpp"
using namespace e2d;

TEST_CASE("math") {
    {
        REQUIRE(math::numeric_cast<u32>(1) == 1u);
        REQUIRE(math::numeric_cast<i32>(1.2f) == 1);
        REQUIRE(math::numeric_cast<i32>(-1.2f) == -1);
        REQUIRE(math::numeric_cast<i32>(1.8f) == 1);
        REQUIRE(math::numeric_cast<i32>(-1.8f) == -1);
    }
    {
        using math::impl::check_numeric_cast;

        // f -> f
        REQUIRE(check_numeric_cast<f64>(std::numeric_limits<f64>::max()));
        REQUIRE(check_numeric_cast<f64>(std::numeric_limits<f64>::lowest()));
        REQUIRE(check_numeric_cast<f64>(std::numeric_limits<f32>::max()));
        REQUIRE(check_numeric_cast<f64>(std::numeric_limits<f32>::lowest()));
        REQUIRE_FALSE(check_numeric_cast<f32>(std::numeric_limits<f64>::max()));
        REQUIRE_FALSE(check_numeric_cast<f32>(std::numeric_limits<f64>::lowest()));
        REQUIRE_FALSE(check_numeric_cast<f64>(std::numeric_limits<f64>::infinity()));
        REQUIRE_FALSE(check_numeric_cast<f64>(std::numeric_limits<f64>::quiet_NaN()));
        REQUIRE_FALSE(check_numeric_cast<f64>(std::numeric_limits<f64>::signaling_NaN()));

        // i -> f
        REQUIRE(check_numeric_cast<f32>(std::numeric_limits<int>::min()));
        REQUIRE(check_numeric_cast<f32>(std::numeric_limits<int>::max()));
        REQUIRE(check_numeric_cast<f64>(std::numeric_limits<int>::min()));
        REQUIRE(check_numeric_cast<f64>(std::numeric_limits<int>::max()));

        // f -> i
        REQUIRE(check_numeric_cast<i32>(1.f));
        REQUIRE(check_numeric_cast<i32>(-1.f));
        REQUIRE_FALSE(check_numeric_cast<i32>(std::numeric_limits<f32>::max()));
        REQUIRE_FALSE(check_numeric_cast<i32>(std::numeric_limits<f32>::lowest()));
        REQUIRE_FALSE(check_numeric_cast<i32>(std::numeric_limits<f32>::infinity()));
        REQUIRE_FALSE(check_numeric_cast<i32>(std::numeric_limits<f32>::quiet_NaN()));
        REQUIRE_FALSE(check_numeric_cast<i32>(std::numeric_limits<f32>::signaling_NaN()));

        // f -> u
        REQUIRE(check_numeric_cast<u32>(1.f));
        REQUIRE_FALSE(check_numeric_cast<u32>(-1.f));
        REQUIRE_FALSE(check_numeric_cast<u32>(std::numeric_limits<f32>::max()));
        REQUIRE_FALSE(check_numeric_cast<u32>(std::numeric_limits<f32>::lowest()));
        REQUIRE_FALSE(check_numeric_cast<u32>(std::numeric_limits<f32>::infinity()));
        REQUIRE_FALSE(check_numeric_cast<u32>(std::numeric_limits<f32>::quiet_NaN()));
        REQUIRE_FALSE(check_numeric_cast<u32>(std::numeric_limits<f32>::signaling_NaN()));

        // i -> i
        REQUIRE(check_numeric_cast<i8>(-128));
        REQUIRE(check_numeric_cast<i8>(127));
        REQUIRE(check_numeric_cast<i16>(i8(-128)));
        REQUIRE(check_numeric_cast<i16>(i8(127)));
        REQUIRE_FALSE(check_numeric_cast<i8>(-129));
        REQUIRE_FALSE(check_numeric_cast<i8>(128));
        REQUIRE_FALSE(check_numeric_cast<i16>(std::numeric_limits<i32>::max()));
        REQUIRE_FALSE(check_numeric_cast<i16>(std::numeric_limits<i32>::lowest()));
        REQUIRE(check_numeric_cast<i32>(std::numeric_limits<i32>::max()));
        REQUIRE(check_numeric_cast<i32>(std::numeric_limits<i32>::lowest()));

        // u -> u
        REQUIRE(check_numeric_cast<u8>(255u));
        REQUIRE_FALSE(check_numeric_cast<u8>(256u));
        REQUIRE_FALSE(check_numeric_cast<u16>(std::numeric_limits<u32>::max()));
        REQUIRE(check_numeric_cast<u16>(std::numeric_limits<u32>::lowest()));
        REQUIRE(check_numeric_cast<u32>(std::numeric_limits<u32>::max()));
        REQUIRE(check_numeric_cast<u32>(std::numeric_limits<u32>::lowest()));

        // i -> u
        REQUIRE(check_numeric_cast<u8>(255));
        REQUIRE_FALSE(check_numeric_cast<u8>(-1));
        REQUIRE_FALSE(check_numeric_cast<u8>(256));

        // u -> i
        REQUIRE(check_numeric_cast<i8>(127u));
        REQUIRE_FALSE(check_numeric_cast<i8>(128u));
    }
    {
        REQUIRE(math::is_power_of_2(1u));
        REQUIRE(math::is_power_of_2(2u));
        REQUIRE(math::is_power_of_2(4u));
        REQUIRE(math::is_power_of_2(8u));
        REQUIRE_FALSE(math::is_power_of_2(0u));
        REQUIRE_FALSE(math::is_power_of_2(3u));
        REQUIRE_FALSE(math::is_power_of_2(6u));
        REQUIRE_FALSE(math::is_power_of_2(10u));

        REQUIRE(math::max_power_of_2<u8>()  == (1u << 7));
        REQUIRE(math::max_power_of_2<u16>() == (1u << 15));
        REQUIRE(math::max_power_of_2<u32>() == (1u << 31));

        REQUIRE(math::next_power_of_2(u8(0)) == u8(1));
        REQUIRE(math::next_power_of_2(u8(1)) == u8(1));
        REQUIRE(math::next_power_of_2(u8(2)) == u8(2));
        REQUIRE(math::next_power_of_2(u16(3)) == u16(4));
        REQUIRE(math::next_power_of_2(u16(4)) == u16(4));
        REQUIRE(math::next_power_of_2(u32(5)) == u32(8));
        REQUIRE(math::next_power_of_2(u32(6)) == u32(8));
        REQUIRE(math::next_power_of_2(u32(8)) == u32(8));
        REQUIRE(math::next_power_of_2(u32(9)) == u32(16));
    }
    {
        REQUIRE(math::is_finite(0));
        REQUIRE(math::is_finite(0u));

        REQUIRE(math::is_finite(0.f));
        REQUIRE(math::is_finite(1.f));
        REQUIRE(math::is_finite(-1.f));

        REQUIRE(math::is_finite(0.0));
        REQUIRE(math::is_finite(1.0));
        REQUIRE(math::is_finite(-1.0));

        REQUIRE_FALSE(math::is_finite(std::numeric_limits<f32>::quiet_NaN()));
        REQUIRE_FALSE(math::is_finite(std::numeric_limits<f32>::signaling_NaN()));
        REQUIRE_FALSE(math::is_finite(std::numeric_limits<f32>::infinity()));
        REQUIRE_FALSE(math::is_finite(-std::numeric_limits<f32>::infinity()));

        REQUIRE_FALSE(math::is_finite(std::numeric_limits<f64>::quiet_NaN()));
        REQUIRE_FALSE(math::is_finite(std::numeric_limits<f64>::signaling_NaN()));
        REQUIRE_FALSE(math::is_finite(std::numeric_limits<f64>::infinity()));
        REQUIRE_FALSE(math::is_finite(-std::numeric_limits<f64>::infinity()));
    }
    {
        REQUIRE(math::abs(2) == 2);
        REQUIRE(math::abs(3u) == 3u);
        REQUIRE(math::abs(-4) == 4);

        REQUIRE(math::approximately(math::abs(1.2f), 1.2f));
        REQUIRE(math::approximately(math::abs(-1.2f), 1.2f));

        REQUIRE(math::approximately(math::abs(2.3), 2.3));
        REQUIRE(math::approximately(math::abs(-2.3), 2.3));

        REQUIRE(math::abs(0.f) == 0.f);
        REQUIRE(math::abs(-0.f) == 0.f);

        REQUIRE(math::abs(0.0) == 0.0);
        REQUIRE(math::abs(-0.0) == 0.0);
    }
    {
        REQUIRE_FALSE(math::sign(0));
        REQUIRE_FALSE(math::sign(1));
        REQUIRE(math::sign(-1));

        REQUIRE_FALSE(math::sign(0u));
        REQUIRE_FALSE(math::sign(1u));

        REQUIRE_FALSE(math::sign(0.f));
        REQUIRE_FALSE(math::sign(0.1f));
        REQUIRE(math::sign(-0.f));
        REQUIRE(math::sign(-0.1f));

        REQUIRE_FALSE(math::sign(0.0));
        REQUIRE_FALSE(math::sign(0.1));
        REQUIRE(math::sign(-0.0));
        REQUIRE(math::sign(-0.1));
    }
    {
        REQUIRE(math::approximately(math::ceil(1), 1));
        REQUIRE(math::approximately(math::floor(1), 1));
        REQUIRE(math::approximately(math::round(1), 1));

        REQUIRE(math::approximately(math::ceil(-1), -1));
        REQUIRE(math::approximately(math::floor(-1), -1));
        REQUIRE(math::approximately(math::round(-1), -1));

        REQUIRE(math::approximately(math::ceil(0.4f), 1.f));
        REQUIRE(math::approximately(math::floor(0.4f), 0.f));
        REQUIRE(math::approximately(math::ceil(-0.4f), 0.f));
        REQUIRE(math::approximately(math::floor(-0.4f), -1.f));

        REQUIRE(math::approximately(math::ceil(0.6f), 1.f));
        REQUIRE(math::approximately(math::floor(0.6f), 0.f));
        REQUIRE(math::approximately(math::ceil(-0.6f), 0.f));
        REQUIRE(math::approximately(math::floor(-0.6f), -1.f));

        REQUIRE(math::approximately(math::round(0.4f), 0.f));
        REQUIRE(math::approximately(math::round(0.6f), 1.f));
        REQUIRE(math::approximately(math::round(-0.4f), 0.f));
        REQUIRE(math::approximately(math::round(-0.6f), -1.f));
    }
    {
        REQUIRE(math::min(1,1) == 1);
        REQUIRE(math::min(1,2) == 1);
        REQUIRE(math::min(2,1) == 1);

        REQUIRE(math::max(1,1) == 1);
        REQUIRE(math::max(1,2) == 2);
        REQUIRE(math::max(2,1) == 2);

        REQUIRE(math::minmax(1,2) == std::make_pair(1,2));
        REQUIRE(math::minmax(2,1) == std::make_pair(1,2));

        REQUIRE(math::clamp(2,1,3) == 2);
        REQUIRE(math::clamp(3,1,3) == 3);
        REQUIRE(math::clamp(4,1,3) == 3);
        REQUIRE(math::clamp(1,1,3) == 1);
        REQUIRE(math::clamp(0,1,3) == 1);

        REQUIRE(math::clamp(2,3,1) == 2);
        REQUIRE(math::clamp(3,3,1) == 3);

        REQUIRE(math::saturate(2) == 1);
        REQUIRE(math::saturate(-1) == 0);
        REQUIRE(math::saturate(0) == 0);
        REQUIRE(math::saturate(1) == 1);
    }
    {
        REQUIRE(math::approximately(math::abs_to_unsigned(1), 1u));
        REQUIRE(math::approximately(math::abs_to_unsigned(-1), 1u));
        REQUIRE(math::approximately(math::abs_to_unsigned(i8(-128)), u8(128)));

        const auto imin = std::numeric_limits<i32>::min();
        const auto umax = std::numeric_limits<u32>::max();
        REQUIRE(math::approximately(math::abs_to_unsigned(imin), umax/2+1));
    }
    {
        REQUIRE(math::is_near_zero(0));
        REQUIRE_FALSE(math::is_near_zero(1));
        REQUIRE_FALSE(math::is_near_zero(1u));
        REQUIRE(math::is_near_zero(1, 1));
        REQUIRE(math::is_near_zero(-1, 1));
        REQUIRE(math::is_near_zero(1u, 1u));

        REQUIRE(math::is_near_zero(0.000001f));
        REQUIRE(math::is_near_zero(-0.000001f));
        REQUIRE_FALSE(math::is_near_zero(0.0001f));
        REQUIRE_FALSE(math::is_near_zero(-0.0001f));
        REQUIRE(math::is_near_zero(0.0001f, 0.001f));
        REQUIRE(math::is_near_zero(-0.0001f, 0.001f));
    }
    {
        static_assert(
            std::is_same<u8, math::make_distance_t<i8>>::value,
            "static unit test error");
        static_assert(
            std::is_same<u8, math::make_distance_t<u8>>::value,
            "static unit test error");
        static_assert(
            std::is_same<f32, math::make_distance_t<f32>>::value,
            "static unit test error");
        static_assert(
            std::is_same<f64, math::make_distance_t<f64>>::value,
            "static unit test error");

        static_assert(
            std::is_same<const u8, math::make_distance_t<const i8>>::value,
            "static unit test error");
        static_assert(
            std::is_same<volatile u8, math::make_distance_t<volatile i8>>::value,
            "static unit test error");
        static_assert(
            std::is_same<const volatile u8, math::make_distance_t<const volatile i8>>::value,
            "static unit test error");
    }
    {
        REQUIRE(math::distance(1, 10) == 9);
        REQUIRE(math::distance(-1, -10) == 9);
        REQUIRE(math::distance(-1, 10) == 11);
        REQUIRE(math::distance(1, -10) == 11);

        REQUIRE(math::distance(10, 3) == 7);
        REQUIRE(math::distance(-10, -3) == 7);
        REQUIRE(math::distance(10, -3) == 13);
        REQUIRE(math::distance(-10, 3) == 13);

        REQUIRE(math::distance(
            std::numeric_limits<i16>::min(),
            i16(0)) == std::numeric_limits<u16>::max()/2 + 1);
        REQUIRE(math::distance(
            std::numeric_limits<i16>::min(),
            i16(-1)) == std::numeric_limits<u16>::max()/2);
        REQUIRE(math::distance(
            std::numeric_limits<i16>::min(),
            std::numeric_limits<i16>::max()) == std::numeric_limits<u16>::max());
        REQUIRE(math::distance(
            std::numeric_limits<i16>::max(),
            std::numeric_limits<i16>::min()) == std::numeric_limits<u16>::max());

        REQUIRE(math::distance(
            std::numeric_limits<i32>::min(),
            i32(0)) == std::numeric_limits<u32>::max()/2 + 1);
        REQUIRE(math::distance(
            std::numeric_limits<i32>::min(),
            i32(-1)) == std::numeric_limits<u32>::max()/2);
        REQUIRE(math::distance(
            std::numeric_limits<i32>::min(),
            std::numeric_limits<i32>::max()) == std::numeric_limits<u32>::max());
        REQUIRE(math::distance(
            std::numeric_limits<i32>::max(),
            std::numeric_limits<i32>::min()) == std::numeric_limits<u32>::max());

        REQUIRE(math::distance(
            std::numeric_limits<i64>::min(),
            i64(0)) == std::numeric_limits<u64>::max()/2 + 1);
        REQUIRE(math::distance(
            std::numeric_limits<i64>::min(),
            i64(-1)) == std::numeric_limits<u64>::max()/2);
        REQUIRE(math::distance(
            std::numeric_limits<i64>::min(),
            std::numeric_limits<i64>::max()) == std::numeric_limits<u64>::max());
        REQUIRE(math::distance(
            std::numeric_limits<i64>::max(),
            std::numeric_limits<i64>::min()) == std::numeric_limits<u64>::max());

        REQUIRE(math::distance(
            u16(0),
            std::numeric_limits<u16>::max()) == std::numeric_limits<u16>::max());
        REQUIRE(math::distance(
            u32(0),
            std::numeric_limits<u32>::max()) == std::numeric_limits<u32>::max());
        REQUIRE(math::distance(
            u64(0),
            std::numeric_limits<u64>::max()) == std::numeric_limits<u64>::max());

        REQUIRE(math::approximately(
            math::distance(std::numeric_limits<f32>::lowest()/2.f, std::numeric_limits<f32>::max()/2.f),
            std::numeric_limits<f32>::max()));
        REQUIRE(math::approximately(
            math::distance(std::numeric_limits<f32>::max()/2.f, std::numeric_limits<f32>::lowest()/2.f),
            std::numeric_limits<f32>::max()));

        REQUIRE(math::approximately(
            math::distance(std::numeric_limits<f64>::lowest()/2.0, std::numeric_limits<f64>::max()/2.0),
            std::numeric_limits<f64>::max()));
        REQUIRE(math::approximately(
            math::distance(std::numeric_limits<f64>::max()/2.0, std::numeric_limits<f64>::lowest()/2.0),
            std::numeric_limits<f64>::max()));
    }
    {
        REQUIRE(math::approximately(1, 1));

        REQUIRE(math::approximately(1, 2, 1));
        REQUIRE(math::approximately(1, 3, 2));
        REQUIRE(math::approximately(1, 3, 3));

        REQUIRE(math::approximately(2, 1, 1));
        REQUIRE(math::approximately(3, 1, 2));
        REQUIRE(math::approximately(3, 1, 3));

        REQUIRE_FALSE(math::approximately(1, 2));
        REQUIRE_FALSE(math::approximately(1, 2, 0));
        REQUIRE_FALSE(math::approximately(1, 3, 1));

        REQUIRE_FALSE(math::approximately(2, 1));
        REQUIRE_FALSE(math::approximately(2, 1, 0));
        REQUIRE_FALSE(math::approximately(3, 1, 1));

        REQUIRE(math::approximately(2u, 3u, 1u));
        REQUIRE(math::approximately(3u, 2u, 1u));

        i32 imin = std::numeric_limits<i32>::min();
        i32 imax = std::numeric_limits<i32>::max();
        REQUIRE_FALSE(math::approximately<i32>(imin,-1,1));
        REQUIRE_FALSE(math::approximately<i32>(imax,-1,1));
        REQUIRE_FALSE(math::approximately<i32>(imin,1,1));
        REQUIRE_FALSE(math::approximately<i32>(imax,1,1));
        REQUIRE_FALSE(math::approximately<i32>(imin,0,1));
        REQUIRE_FALSE(math::approximately<i32>(imax,0,1));
        REQUIRE_FALSE(math::approximately<i32>(imin,imax,1));
        REQUIRE_FALSE(math::approximately<i32>(imax,imin,1));

        u32 umin = std::numeric_limits<u32>::min();
        u32 umax = std::numeric_limits<u32>::max();
        REQUIRE_FALSE(math::approximately<u32>(umax,1,1));
        REQUIRE_FALSE(math::approximately<u32>(umax,0,1));
        REQUIRE_FALSE(math::approximately<u32>(umin,umax,1));
        REQUIRE_FALSE(math::approximately<u32>(umax,umin,1));
    }
}
