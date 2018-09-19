/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/window.hpp>

namespace e2d
{
    class window::state final : private e2d::noncopyable {
    public:
    };

    window::window()
    : state_(new state()){}

    window::~window() noexcept = default;
}
