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
        {
            auto nreq = n.request(net::request_builder()
                .url("http://httpbin.org/status/201"));

            nreq.promise.then([](const net::response& response){
                REQUIRE(response.http_code() == 201u);
            }).except([](std::exception_ptr){
                REQUIRE(false);
            }).wait();
        }
        {
            auto nreq = n.request(net::request_builder()
                .url("http://httpbin.org/delay/2"));

            REQUIRE(nreq.request.cancel());

            nreq.promise.then([](const net::response&){
                REQUIRE(false);
            }).except([](std::exception_ptr e){
                try {
                    std::rethrow_exception(e);
                } catch (const request_cancelled_exception&) {
                    REQUIRE(true);
                } catch (...) {
                    REQUIRE(false);
                }
            }).wait();
        }
    }
}
