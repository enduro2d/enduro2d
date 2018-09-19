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
        bool should_close = false;
        std::mutex mutex;
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

    void window::hide() noexcept {
    }

    void window::show() noexcept {
    }

    void window::restore() noexcept {
    }

    void window::minimize() noexcept {
    }

    v2u window::real_size() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->size;
    }

    v2u window::virtual_size() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->size;
    }

    const str& window::title() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->title;
    }

    void window::set_title(str_view title) {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->title = make_utf8(title);
    }

    bool window::should_close() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->should_close;
    }

    void window::set_should_close(bool yesno) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->should_close = yesno;
    }

    void window::swap_buffers() noexcept {
    }

    bool window::poll_events() noexcept {
        return false;
    }
}

#endif
