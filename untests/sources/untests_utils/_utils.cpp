/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#define CATCH_CONFIG_MAIN
#include "_utils.hpp"
using namespace e2d;

TEST_CASE("utils") {
    {
        REQUIRE(utils::sdbm_hash("") == 0u);
        REQUIRE(utils::sdbm_hash(1u, "") == 1u);

        REQUIRE(utils::sdbm_hash<char>(nullptr, nullptr) == 0u);
        REQUIRE(utils::sdbm_hash<char>(1u, nullptr, nullptr) == 1u);

        const char* str1 = "hello";
        const char* str2 = "hello";

        REQUIRE(utils::sdbm_hash(str1) == utils::sdbm_hash(str2));
        REQUIRE(utils::sdbm_hash(42u, str1) == utils::sdbm_hash(42u, str2));

        REQUIRE(utils::sdbm_hash(
            str1, str1 + std::strlen(str1)
        ) == utils::sdbm_hash(str2));
        REQUIRE(utils::sdbm_hash(
            42u, str1, str1 + std::strlen(str1)
        ) == utils::sdbm_hash(42u, str2));
    }
}
