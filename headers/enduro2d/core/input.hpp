/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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

        v2f cursor_pos() const noexcept;
        v2f scroll_delta() const noexcept;

        bool is_any_button_pressed() const noexcept;
        bool is_any_button_just_pressed() const noexcept;
        bool is_any_button_just_released() const noexcept;

        bool is_button_pressed(mouse_button btn) const noexcept;
        bool is_button_just_pressed(mouse_button btn) const noexcept;
        bool is_button_just_released(mouse_button btn) const noexcept;

        void extract_pressed_buttons(vector<mouse_button>& dst) const;
        void extract_just_pressed_buttons(vector<mouse_button>& dst) const;
        void extract_just_released_buttons(vector<mouse_button>& dst) const;
    private:
        friend class input;
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };

    class keyboard final : private noncopyable {
    public:
        keyboard();
        ~keyboard() noexcept;

        str32 input_text() const;
        void extract_input_text(str32& dst) const;

        bool is_any_key_pressed() const noexcept;
        bool is_any_key_just_pressed() const noexcept;
        bool is_any_key_just_released() const noexcept;

        bool is_key_pressed(keyboard_key key) const noexcept;
        bool is_key_just_pressed(keyboard_key key) const noexcept;
        bool is_key_just_released(keyboard_key key) const noexcept;

        void extract_pressed_keys(vector<keyboard_key>& dst) const;
        void extract_just_pressed_keys(vector<keyboard_key>& dst) const;
        void extract_just_released_keys(vector<keyboard_key>& dst) const;
    private:
        friend class input;
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };

    class input final : public module<input> {
    public:
        struct input_char_event {
            char32_t uchar{0};
            input_char_event(char32_t nuchar)
            : uchar(nuchar) {}
        };
        struct move_cursor_event {
            v2f pos{v2f::zero()};
            move_cursor_event(const v2f& npos)
            : pos(npos) {}
        };
        struct mouse_scroll_event {
            v2f delta{v2f::zero()};
            mouse_scroll_event(const v2f& ndelta)
            : delta(ndelta) {}
        };
        struct mouse_button_event {
            mouse_button button{mouse_button::unknown};
            mouse_button_action action{mouse_button_action::unknown};
            mouse_button_event(mouse_button nbutton, mouse_button_action naction)
            : button(nbutton)
            , action(naction) {}

        };
        struct keyboard_key_event {
            keyboard_key key{keyboard_key::unknown};
            keyboard_key_action action{keyboard_key_action::unknown};
            keyboard_key_event(keyboard_key nkey, keyboard_key_action naction)
            : key(nkey)
            , action(naction) {}
        };
    public:
        input();
        ~input() noexcept final;

        const class mouse& mouse() const noexcept;
        const class keyboard& keyboard() const noexcept;

        void post_event(input_char_event evt) noexcept;
        void post_event(move_cursor_event evt) noexcept;
        void post_event(mouse_scroll_event evt) noexcept;
        void post_event(mouse_button_event evt) noexcept;
        void post_event(keyboard_key_event evt) noexcept;

        void frame_tick() noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };

    class window_input_source : public window::event_listener {
    public:
        window_input_source(input& input) noexcept;
        void on_input_char(char32_t uchar) noexcept final;
        void on_move_cursor(const v2f& pos) noexcept final;
        void on_mouse_scroll(const v2f& delta) noexcept final;
        void on_mouse_button(mouse_button btn, mouse_button_action act) noexcept final;
        void on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept final;
    private:
        input& input_;
    };
}
