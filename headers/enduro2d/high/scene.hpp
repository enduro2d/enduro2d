/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class node final : private noncopyable {
    public:
        node();
        ~node() noexcept;
    };

    class scene final : private noncopyable {
    public:
        scene();
        ~scene() noexcept;
    };
}
