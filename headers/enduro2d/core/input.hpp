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

        bool any_key_up() const noexcept;
        bool any_key_down() const noexcept;
        bool any_key_downed() const noexcept;

        bool any_mouse_up() const noexcept;
        bool any_mouse_down() const noexcept;
        bool any_mouse_downed() const noexcept;

        bool key_up(key key) const noexcept;
        bool key_down(key key) const noexcept;
        bool key_downed(key key) const noexcept;

        bool mouse_up(mouse mouse) const noexcept;
        bool mouse_down(mouse mouse) const noexcept;
        bool mouse_downed(mouse mouse) const noexcept;

        void extract_up_keys(std::vector<key>& result) const;
        void extract_down_keys(std::vector<key>& result) const;
        void extract_downed_keys(std::vector<key>& result) const;

        void frame_tick() noexcept;
        void post_event(key key, key_action action) noexcept;
        void post_event(mouse mouse, mouse_action action) noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };

    class window_input_source : public window::event_listener {
    public:
        window_input_source(input& input) noexcept;
        void on_key(key key, u32 scancode, key_action action) noexcept final;
        void on_mouse(mouse mouse, mouse_action action) noexcept final;
    private:
        input& input_;
    };
}
