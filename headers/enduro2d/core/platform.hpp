/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class platform final : public module<platform> {
    public:
        platform(int argc, char *argv[]);
        ~platform() noexcept final;

        std::size_t command_line_argument_count() const noexcept;
        const str& command_line_argument(std::size_t index) const noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
