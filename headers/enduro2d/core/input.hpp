/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class bad_input_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad input operation";
        }
    };

    class input final : public module<input> {
    public:
        input();
        ~input() noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };
}
