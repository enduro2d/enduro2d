/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("time") {
    {
        REQUIRE(time::second<i32>().value == 1);
        REQUIRE(time::second_ms<i32>().value == 1000);
        REQUIRE(time::second_us<i32>().value == 1'000'000);

        REQUIRE(time::minute<i32>().value == 60);
        REQUIRE(time::minute_ms<i32>().value == 60000);
        REQUIRE(time::minute_us<i32>().value == 60'000'000);

        REQUIRE(time::hour<i32>().value == 60 * 60);
        REQUIRE(time::hour_ms<i32>().value == 60 * 60 * 1000);
        REQUIRE(time::hour_us<i64>().value == 60 * 60 * i64(1'000'000));
    }
    {
        REQUIRE(make_seconds(1).convert_to<milliseconds_tag>().value == 1000);
        REQUIRE(make_seconds(1).convert_to<microseconds_tag>().value == 1000000);

        REQUIRE(make_milliseconds(1000).convert_to<seconds_tag>().value == 1);
        REQUIRE(make_milliseconds(1000).convert_to<microseconds_tag>().value == 1000000);

        REQUIRE(make_microseconds(1000000).convert_to<seconds_tag>().value == 1);
        REQUIRE(make_microseconds(1000000).convert_to<milliseconds_tag>().value == 1000);
    }
    {
        namespace ch = std::chrono;

        REQUIRE(time::to_chrono(make_seconds<i8>(2)) == ch::seconds(2));
        REQUIRE(time::to_chrono(make_milliseconds<i8>(3)) == ch::milliseconds(3));
        REQUIRE(time::to_chrono(make_microseconds<i8>(4)) == ch::microseconds(4));

        REQUIRE(time::to_chrono(make_seconds<u8>(2)) == ch::seconds(2));
        REQUIRE(time::to_chrono(make_milliseconds<u8>(3)) == ch::milliseconds(3));
        REQUIRE(time::to_chrono(make_microseconds<u8>(4)) == ch::microseconds(4));
    }
    {
        REQUIRE(time::to_seconds(make_seconds(2)).value == 2);
        REQUIRE(time::to_milliseconds(make_seconds(2)).value == 2000);
        REQUIRE(time::to_microseconds(make_seconds(2)).value == 2000000);

        REQUIRE(time::to_seconds(make_milliseconds(2000)).value == 2);
        REQUIRE(time::to_milliseconds(make_milliseconds(2000)).value == 2000);
        REQUIRE(time::to_microseconds(make_milliseconds(2000)).value == 2000000);

        REQUIRE(time::to_seconds(make_microseconds(2000000)).value == 2);
        REQUIRE(time::to_milliseconds(make_microseconds(2000000)).value == 2000);
        REQUIRE(time::to_microseconds(make_microseconds(2000000)).value == 2000000);
    }
    {
        auto b = time::now<milliseconds_tag>();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto e = time::now<milliseconds_tag>();
        REQUIRE(e > b);
    }
}
