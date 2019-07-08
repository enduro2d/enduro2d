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
    // network
    //

    class network final : public module<network> {
    public:
        network();
        ~network() noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
