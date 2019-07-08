/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core.hpp"
using namespace e2d;

TEST_CASE("network"){
    network n;

    SECTION("request"){
        auto request = net::request_builder()
            .url("http://httpbin.org/status/201")
            .send();
        REQUIRE(request.take().http_code() == 201u);
    }
}
