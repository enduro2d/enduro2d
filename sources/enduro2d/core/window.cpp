/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/window.hpp>
#include <enduro2d/core/debug.hpp>

namespace e2d
{
    //
    // class window::event_listener
    //

    void window::event_listener::on_key(key key, u32 scancode, key_action action) noexcept {
        E2D_UNUSED(key, scancode, action);
    }

    void window::event_listener::on_uchar(char32_t uchar) noexcept {
        E2D_UNUSED(uchar);
    }

    void window::event_listener::on_scroll(const v2f& delta) noexcept {
        E2D_UNUSED(delta);
    }

    void window::event_listener::on_cursor(const v2f& position) noexcept {
        E2D_UNUSED(position);
    }

    void window::event_listener::on_mouse(mouse mouse, mouse_action action) noexcept {
        E2D_UNUSED(mouse, action);
    }

    void window::event_listener::on_focus(bool focused) noexcept {
        E2D_UNUSED(focused);
    }

    void window::event_listener::on_minimize(bool minimized) noexcept {
        E2D_UNUSED(minimized);
    }

    //
    // class trace_window_event_listener
    //

    window_trace_event_listener::window_trace_event_listener(debug& debug) noexcept
    : debug_(debug) {}

    void window_trace_event_listener::on_key(key key, u32 scancode, key_action action) noexcept {
        debug_.trace("WINDOW: on_key(key: %0 scancode: %1 action: %2)",
            key_to_cstr(key),
            scancode,
            key_action_to_cstr(action));
    }

    void window_trace_event_listener::on_uchar(char32_t uchar) noexcept {
        debug_.trace("WINDOW: on_uchar(uchar: %0)", str32_view(&uchar, 1));
    }

    void window_trace_event_listener::on_scroll(const v2f& delta) noexcept {
        debug_.trace("WINDOW: on_scroll(delta: %0)", delta);
    }

    void window_trace_event_listener::on_cursor(const v2f& position) noexcept {
        debug_.trace("WINDOW: on_cursor(position: %0)", position);
    }

    void window_trace_event_listener::on_mouse(mouse mouse, mouse_action action) noexcept {
        debug_.trace("WINDOW: on_mouse(mouse: %0 action: %1)",
            mouse_to_cstr(mouse),
            mouse_action_to_cstr(action));
    }

    void window_trace_event_listener::on_focus(bool focused) noexcept {
        debug_.trace("WINDOW: on_focus(focused: %0)", focused);
    }

    void window_trace_event_listener::on_minimize(bool minimized) noexcept {
        debug_.trace("WINDOW: on_minimize(minimized: %0)", minimized);
    }
}
