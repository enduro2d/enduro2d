/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_base.hpp"
using namespace e2d;

TEST_CASE("macros") {
    SECTION("E2D_CLEAR_ALLOCA") {
        const u8* m = static_cast<u8*>(E2D_CLEAR_ALLOCA(32));
        for ( std::size_t i = 0; i < 32; ++i ) {
            REQUIRE_FALSE(m[i]);
        }
    }
    SECTION("E2D_COUNTOF") {
        u32 arr[12];
        u32 arr2[E2D_COUNTOF(arr)];
        REQUIRE(E2D_COUNTOF(arr) == 12);
        REQUIRE(E2D_COUNTOF(arr2) == 12);
    }
}
