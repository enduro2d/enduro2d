/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_MAIN
#include "_utils.hpp"
using namespace e2d;

TEST_CASE("utils") {
    {
        enum class ee_u8 : u8 {
            ee1 = 10,
            ee2 = 42
        };
        enum class ee_i16 : i16 {
            ee1 = 10,
            ee2 = 42
        };
        auto e1 = utils::enum_to_underlying(ee_u8::ee1);
        auto e2 = utils::enum_to_underlying(ee_i16::ee2);
        REQUIRE(e1 == u8(10));
        REQUIRE(e2 == i16(42));
        static_assert(
            std::is_same<u8, decltype(e1)>::value,
            "static unit test error");
        static_assert(
            std::is_same<i16, decltype(e2)>::value,
            "static unit test error");
    }
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
    {
        utils::type_family_id id1 = utils::type_family<str16>::id();
        utils::type_family_id id2 = utils::type_family<str32>::id();
        REQUIRE(id1 != id2);
        REQUIRE(id1 == utils::type_family<str16>::id());
        REQUIRE(id2 == utils::type_family<str32>::id());
    }
}
