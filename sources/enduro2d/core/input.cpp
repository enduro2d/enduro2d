/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/input.hpp>

namespace e2d
{
    class input::state final : private e2d::noncopyable {
    public:
        state() = default;
        ~state() noexcept = default;
    };

    input::input()
    : state_(new state()) {}
    input::~input() noexcept = default;
}
