/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class platform final : public module<platform> {
    public:
        platform();
        ~platform() noexcept final;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
