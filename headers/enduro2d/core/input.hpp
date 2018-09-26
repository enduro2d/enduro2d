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
    class bad_input_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad input operation";
        }
    };

    class input final : public module<input> {
    public:
        input();
        ~input() noexcept;

        bool is_any_mouse_button_pressed() const noexcept;
        bool is_any_mouse_button_just_pressed() const noexcept;
        bool is_any_mouse_button_just_released() const noexcept;

        bool is_any_keyboard_key_pressed() const noexcept;
        bool is_any_keyboard_key_just_pressed() const noexcept;
        bool is_any_keyboard_key_just_released() const noexcept;

        bool is_mouse_button_pressed(mouse_button btn) const noexcept;
        bool is_mouse_button_just_pressed(mouse_button btn) const noexcept;
        bool is_mouse_button_just_released(mouse_button btn) const noexcept;

        bool is_keyboard_key_pressed(keyboard_key key) const noexcept;
        bool is_keyboard_key_just_pressed(keyboard_key key) const noexcept;
        bool is_keyboard_key_just_released(keyboard_key key) const noexcept;

        void extract_pressed_mouse_buttons(std::vector<mouse_button>& dst) const;
        void extract_just_pressed_mouse_buttons(std::vector<mouse_button>& dst) const;
        void extract_just_released_mouse_buttons(std::vector<mouse_button>& dst) const;

        void extract_pressed_keyboard_keys(std::vector<keyboard_key>& dst) const;
        void extract_just_pressed_keyboard_keys(std::vector<keyboard_key>& dst) const;
        void extract_just_released_keyboard_keys(std::vector<keyboard_key>& dst) const;

        void frame_tick() noexcept;
        void post_event(keyboard_key key, keyboard_key_action act) noexcept;
        void post_event(mouse_button btn, mouse_button_action act) noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };

    class window_input_source : public window::event_listener {
    public:
        window_input_source(input& input) noexcept;
        void on_mouse_button(mouse_button btn, mouse_button_action act) noexcept final;
        void on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept final;
    private:
        input& input_;
    };
}
