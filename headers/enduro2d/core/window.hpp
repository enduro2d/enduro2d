/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class window final : public module<window> {
    public:
        window();
        ~window() noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };
}
