/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "window.hpp"

#if defined(E2D_WINDOW_MODE) && E2D_WINDOW_MODE == E2D_WINDOW_MODE_NONE

namespace e2d
{
    class window::state final : private e2d::noncopyable {
    public:
        v2u size;
        str title;
        bool fullscreen = false;
    public:
        state(const v2u& size, str_view title, bool fullscreen)
        : size(size)
        , title(make_utf8(title))
        , fullscreen(fullscreen) {}
        ~state() noexcept = default;
    };

    window::window(const v2u& size, str_view title, bool fullscreen)
    : state_(new state(size, title, fullscreen)) {}
    window::~window() noexcept = default;

    v2u window::real_size() const noexcept {
        return state_->size;
    }

    v2u window::virtual_size() const noexcept {
        return state_->size;
    }

    bool window::should_close() const noexcept {
        return false;
    }

    void window::swap_buffers() noexcept {
    }

    bool window::poll_events() noexcept {
        return false;
    }
}

#endif
