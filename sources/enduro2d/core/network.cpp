/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/network.hpp>

namespace e2d
{
    class network::internal_state final : private e2d::noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;
    private:
        net::performer performer_;
    };
}

namespace e2d
{
    network::network()
    : state_(new internal_state()) {}
    network::~network() noexcept = default;

    network_request network::request(net::request_builder& rb) {
        return request(std::move(rb));
    }

    network_request network::request(net::request_builder&& rb) {
        if ( rb.callback() ) {
            throw bad_network_operation();
        }
        stdex::promise<net::response> promise;
        rb.callback([promise](net::request request) mutable {
            if ( request.is_done() ) {
                promise.resolve(request.take());
                return;
            }
            if ( request.status() == net::req_status::cancelled ) {
                promise.reject(request_cancelled_exception());
            } else {
                promise.reject(request_network_error(request.get_error()));
            }
        });
        return {rb.send(), std::move(promise)};
    }
}
