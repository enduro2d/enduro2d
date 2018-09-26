/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class bad_window_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad window operation";
        }
    };

    class window final : public module<window> {
    public:
        class event_listener : private e2d::noncopyable {
        public:
            virtual ~event_listener() noexcept = default;
            virtual void on_scroll(const v2f& delta) noexcept;
            virtual void on_move_cursor(const v2f& pos) noexcept;
            virtual void on_input_char(char32_t uchar) noexcept;
            virtual void on_mouse_button(mouse_button btn, mouse_button_action act) noexcept;
            virtual void on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept;
            virtual void on_window_close() noexcept;
            virtual void on_window_focus(bool focused) noexcept;
            virtual void on_window_minimize(bool minimized) noexcept;
        };
        using event_listener_uptr = std::unique_ptr<event_listener>;
    public:
        window(const v2u& size, str_view title, bool vsync, bool fullscreen);
        ~window() noexcept;

        void hide() noexcept;
        void show() noexcept;
        void restore() noexcept;
        void minimize() noexcept;

        bool visible() const noexcept;
        bool focused() const noexcept;
        bool minimized() const noexcept;

        bool vsync() const noexcept;
        bool fullscreen() const noexcept;

        bool toggle_vsync(bool yesno) noexcept;
        bool toggle_fullscreen(bool yesno) noexcept;

        v2u real_size() const noexcept;
        v2u virtual_size() const noexcept;
        v2u framebuffer_size() const noexcept;

        const str& title() const noexcept;
        void set_title(str_view title);

        bool should_close() const noexcept;
        void set_should_close(bool yesno) noexcept;

        void swap_buffers() noexcept;
        static bool frame_tick() noexcept;

        template < typename T, typename... Args >
        T& register_event_listener(Args&&... args);
        event_listener& register_event_listener(event_listener_uptr listener);
        void unregister_event_listener(const event_listener& listener) noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };

    class window_trace_event_listener final : public window::event_listener {
    public:
        window_trace_event_listener(debug& debug) noexcept;
        void on_scroll(const v2f& delta) noexcept final;
        void on_move_cursor(const v2f& pos) noexcept final;
        void on_input_char(char32_t uchar) noexcept final;
        void on_mouse_button(mouse_button btn, mouse_button_action act) noexcept final;
        void on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept final;
        void on_window_close() noexcept final;
        void on_window_focus(bool focused) noexcept final;
        void on_window_minimize(bool minimized) noexcept final;
    private:
        debug& debug_;
    };
}

namespace e2d
{
    template < typename T, typename... Args >
    T& window::register_event_listener(Args&&... args) {
        return static_cast<T&>(
            register_event_listener(std::make_unique<T>(std::forward<Args>(args)...)));
    }
}
