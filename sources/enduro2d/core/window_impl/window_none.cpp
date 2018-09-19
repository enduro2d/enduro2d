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
        v2u virtual_size;
        str title;
        bool vsync = false;
        bool fullscreen = false;
        bool should_close = false;
        bool visible = true;
        bool focused = true;
        bool minimized = false;
        std::mutex mutex;
    public:
        state(const v2u& size, str_view title, bool vsync, bool fullscreen)
        : virtual_size(size)
        , title(make_utf8(title))
        , vsync(vsync)
        , fullscreen(fullscreen) {}
        ~state() noexcept = default;
    };

    window::window(const v2u& size, str_view title, bool vsync, bool fullscreen)
    : state_(new state(size, title, vsync, fullscreen)) {}
    window::~window() noexcept = default;

    void window::hide() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->visible = false;
    }

    void window::show() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->visible = true;
    }

    void window::restore() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->minimized = false;
    }

    void window::minimize() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->minimized = true;
    }

    bool window::visible() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->visible;
    }

    bool window::focused() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->focused;
    }

    bool window::minimized() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->minimized;
    }

    bool window::vsync() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->vsync;
    }

    bool window::fullscreen() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->fullscreen;
    }

    bool window::toggle_vsync(bool yesno) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->vsync = yesno;
        return true;
    }

    bool window::toggle_fullscreen(bool yesno) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->fullscreen = yesno;
        return true;
    }

    v2u window::real_size() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->virtual_size;
    }

    v2u window::virtual_size() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->virtual_size;
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
