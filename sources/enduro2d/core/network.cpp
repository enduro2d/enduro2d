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
}
