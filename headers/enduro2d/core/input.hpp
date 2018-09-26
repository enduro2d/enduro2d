/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"
#include "window.hpp"

namespace e2d
{
    class mouse final : private noncopyable {
    public:
        mouse();
        ~mouse() noexcept;

        bool is_any_button_pressed() const noexcept;
        bool is_any_button_just_pressed() const noexcept;
        bool is_any_button_just_released() const noexcept;

        bool is_button_pressed(mouse_button btn) const noexcept;
        bool is_button_just_pressed(mouse_button btn) const noexcept;
        bool is_button_just_released(mouse_button btn) const noexcept;

        void extract_pressed_buttons(std::vector<mouse_button>& dst) const;
        void extract_just_pressed_buttons(std::vector<mouse_button>& dst) const;
        void extract_just_released_buttons(std::vector<mouse_button>& dst) const;
    private:
        class state;
        friend class input;
        std::unique_ptr<state> state_;
    };

    class keyboard final : private noncopyable {
    public:
        keyboard();
        ~keyboard() noexcept;

        bool is_any_key_pressed() const noexcept;
        bool is_any_key_just_pressed() const noexcept;
        bool is_any_key_just_released() const noexcept;

        bool is_key_pressed(keyboard_key key) const noexcept;
        bool is_key_just_pressed(keyboard_key key) const noexcept;
        bool is_key_just_released(keyboard_key key) const noexcept;

        void extract_pressed_keys(std::vector<keyboard_key>& dst) const;
        void extract_just_pressed_keys(std::vector<keyboard_key>& dst) const;
        void extract_just_released_keys(std::vector<keyboard_key>& dst) const;
    private:
        class state;
        friend class input;
        std::unique_ptr<state> state_;
    };

    class input final : public module<input> {
    public:
        struct input_char_event {
            char32_t uchar = 0;
        };
        struct mouse_button_event {
            mouse_button button = mouse_button::unknown;
            mouse_button_action action = mouse_button_action::unknown;
        };
        struct keyboard_key_event {
            keyboard_key key = keyboard_key::unknown;
            keyboard_key_action action = keyboard_key_action::unknown;
        };
    public:
        input();
        ~input() noexcept;

        str32 input_text() const;
        void extract_input_text(str32& dst) const;

        const mouse& mouse() const noexcept;
        const keyboard& keyboard() const noexcept;

        void post_event(input_char_event evt) noexcept;
        void post_event(mouse_button_event evt) noexcept;
        void post_event(keyboard_key_event evt) noexcept;

        void frame_tick() noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };

    class window_input_source : public window::event_listener {
    public:
        window_input_source(input& input) noexcept;
        void on_input_char(char32_t uchar) noexcept final;
        void on_mouse_button(mouse_button btn, mouse_button_action act) noexcept final;
        void on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept final;
    private:
        input& input_;
    };
}
