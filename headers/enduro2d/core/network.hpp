/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    //
    // bad_network_operation
    //

    class bad_network_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad network operation";
        }
    };

    //
    // request_network_error
    //

    class request_network_error final : public exception {
    public:
        request_network_error() = default;

        request_network_error(str error)
        : error_(std::move(error)) {}

        const char* what() const noexcept final {
            return error_.empty()
                ? "request network error"
                : error_.c_str();
        }
    private:
        str error_;
    };

    //
    // request_cancelled_exception
    //

    class request_cancelled_exception : public exception {
    public:
        const char* what() const noexcept override {
            return "request cancelled exception";
        }
    };

    //
    // network_request
    //

    struct network_request final {
        net::request request;
        stdex::promise<net::response> promise;
    };

    //
    // network
    //

    class network final : public module<network> {
    public:
        network();
        ~network() noexcept;

        network_request request(net::request_builder& rb);
        network_request request(net::request_builder&& rb);
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
