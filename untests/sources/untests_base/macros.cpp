/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
}
