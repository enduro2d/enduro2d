/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

TEST_CASE("address") {
    SECTION("parent") {
        REQUIRE(address::parent("") == "");
        REQUIRE(address::parent(":/") == "");
        REQUIRE(address::parent(":/child") == "");
        REQUIRE(address::parent("at/las.json") == "at/las.json");
        REQUIRE(address::parent("at/las.json:/") == "at/las.json");
        REQUIRE(address::parent("at/las.json:/spr/ite.png") == "at/las.json");
        REQUIRE(address::parent("at/las.json:/spr/ite.png:/") == "at/las.json");
        REQUIRE(address::parent("at/las.json:/spr/ite.png:/child") == "at/las.json");
    }
    SECTION("nested") {
        REQUIRE(address::nested("") == "");
        REQUIRE(address::nested(":/") == "");
        REQUIRE(address::nested(":/chi/ld") == "chi/ld");
        REQUIRE(address::nested("at/las.json") == "");
        REQUIRE(address::nested("at/las.json:/") == "");
        REQUIRE(address::nested("at/las.json:/spr/ite.png") == "spr/ite.png");
        REQUIRE(address::nested("at/las.json:/spr/ite.png:/") == "spr/ite.png:/");
        REQUIRE(address::nested("at/las.json:/spr/ite.png:/chi/ld") == "spr/ite.png:/chi/ld");
    }
}
