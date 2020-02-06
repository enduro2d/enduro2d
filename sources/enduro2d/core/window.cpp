/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/window.hpp>

#include <enduro2d/core/debug.hpp>
#include <enduro2d/core/input.hpp>

namespace e2d
{
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

    void window::event_listener::on_window_scale(const v2f& scale) noexcept {
        E2D_UNUSED(scale);
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
}

namespace e2d
{
    window_event_tracer::window_event_tracer(debug& debug) noexcept
    : debug_(debug) {}

    void window_event_tracer::on_input_char(char32_t uchar) noexcept {
        debug_.trace("WINDOW: on_input_char(uchar: %0)", str32_view(&uchar, 1));
    }

    void window_event_tracer::on_move_cursor(const v2f& pos) noexcept {
        debug_.trace("WINDOW: on_move_cursor(pos: %0)", pos);
    }

    void window_event_tracer::on_mouse_scroll(const v2f& delta) noexcept {
        debug_.trace("WINDOW: on_scroll(delta: %0)", delta);
    }

    void window_event_tracer::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        debug_.trace("WINDOW: on_mouse_button(btn: %0 act: %1)", btn, act);
    }

    void window_event_tracer::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        debug_.trace("WINDOW: on_keyboard_key(key: %0 scancode: %1 act: %2)", key, scancode, act);
    }

    void window_event_tracer::on_window_size(const v2u& size) noexcept {
        debug_.trace("WINDOW: on_window_size(size: %0)", size);
    }

    void window_event_tracer::on_window_scale(const v2f& scale) noexcept {
        debug_.trace("WINDOW: on_window_scale(scale: %0)", scale);
    }

    void window_event_tracer::on_framebuffer_size(const v2u& size) noexcept {
        debug_.trace("WINDOW: on_framebuffer_size(size: %0)", size);
    }

    void window_event_tracer::on_window_close() noexcept {
        debug_.trace("WINDOW: on_window_close()");
    }

    void window_event_tracer::on_window_focus(bool focused) noexcept {
        debug_.trace("WINDOW: on_window_focus(focused: %0)", focused);
    }

    void window_event_tracer::on_window_minimize(bool minimized) noexcept {
        debug_.trace("WINDOW: on_window_minimize(minimized: %0)", minimized);
    }
}

namespace e2d
{
    window_input_source::window_input_source(input& input) noexcept
    : input_(input) {}

    void window_input_source::on_input_char(char32_t uchar) noexcept {
        input_.post_event(input::input_char_event{uchar});
    }

    void window_input_source::on_move_cursor(const v2f& pos) noexcept {
        input_.post_event(input::move_cursor_event{pos});
    }

    void window_input_source::on_mouse_scroll(const v2f& delta) noexcept {
        input_.post_event(input::mouse_scroll_event{delta});
    }

    void window_input_source::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        input_.post_event(input::mouse_button_event{btn, act});
    }

    void window_input_source::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        E2D_UNUSED(scancode);
        input_.post_event(input::keyboard_key_event{key, act});
    }
}
