/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render.hpp"

#if defined(E2D_RENDER_MODE) && E2D_RENDER_MODE == E2D_RENDER_MODE_NONE

namespace e2d
{
    class render::state final : private e2d::noncopyable {
    public:
        state() {}
        ~state() noexcept = default;
    };

    render::render()
    : state_(new state()) {}
    render::~render() noexcept = default;
}

#endif
