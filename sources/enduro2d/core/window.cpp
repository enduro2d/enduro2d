/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/window.hpp>

#include <enduro2d/core/debug.hpp>

namespace e2d
{
    //
    // class window::event_listener
    //

    void window::event_listener::on_input_char(char32_t uchar) noexcept {
        E2D_UNUSED(uchar);
    }

    void window::event_listener::on_move_cursor(const v2f& pos) noexcept {
        E2D_UNUSED(pos);
    }

    void window::event_listener::on_mouse_scroll(const v2f& delta) noexcept {
        E2D_UNUSED(delta);
    }

    void window::event_listener::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        E2D_UNUSED(btn, act);
    }

    void window::event_listener::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        E2D_UNUSED(key, scancode, act);
    }

    void window::event_listener::on_window_size(const v2u& size) noexcept {
        E2D_UNUSED(size);
    }

    void window::event_listener::on_framebuffer_size(const v2u& size) noexcept {
        E2D_UNUSED(size);
    }

    void window::event_listener::on_window_close() noexcept {
    }

    void window::event_listener::on_window_focus(bool focused) noexcept {
        E2D_UNUSED(focused);
    }

    void window::event_listener::on_window_minimize(bool minimized) noexcept {
        E2D_UNUSED(minimized);
    }

    //
    // class trace_window_event_listener
    //

    window_trace_event_listener::window_trace_event_listener(debug& debug) noexcept
    : debug_(debug) {}

    void window_trace_event_listener::on_input_char(char32_t uchar) noexcept {
        debug_.trace("WINDOW: on_input_char(uchar: %0)", str32_view(&uchar, 1));
    }

    void window_trace_event_listener::on_move_cursor(const v2f& pos) noexcept {
        debug_.trace("WINDOW: on_move_cursor(pos: %0)", pos);
    }

    void window_trace_event_listener::on_mouse_scroll(const v2f& delta) noexcept {
        debug_.trace("WINDOW: on_scroll(delta: %0)", delta);
    }

    void window_trace_event_listener::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        debug_.trace("WINDOW: on_mouse_button(btn: %0 act: %1)",
            btn,
            act);
    }

    void window_trace_event_listener::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        debug_.trace("WINDOW: on_keyboard_key(key: %0 scancode: %1 act: %2)",
            key,
            scancode,
            act);
    }

    void window_trace_event_listener::on_window_size(const v2u& size) noexcept {
        debug_.trace("WINDOW: on_window_size(size: %0)", size);
    }

    void window_trace_event_listener::on_framebuffer_size(const v2u& size) noexcept {
        debug_.trace("WINDOW: on_framebuffer_size(size: %0)", size);
    }

    void window_trace_event_listener::on_window_close() noexcept {
        debug_.trace("WINDOW: on_window_close()");
    }

    void window_trace_event_listener::on_window_focus(bool focused) noexcept {
        debug_.trace("WINDOW: on_window_focus(focused: %0)", focused);
    }

    void window_trace_event_listener::on_window_minimize(bool minimized) noexcept {
        debug_.trace("WINDOW: on_window_minimize(minimized: %0)", minimized);
    }
}
