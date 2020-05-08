/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "touch_system_base.hpp"

namespace e2d::touch_system_impl
{
    class collector final : public window::event_listener {
    public:
        struct mouse_move_event {
            v2f pos{v2f::zero()};
            mouse_move_event(const v2f& npos)
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
    public:
        using event = std::variant<std::monostate,
            mouse_move_event,
            mouse_scroll_event,
            mouse_button_event>;
        using events = vector<event>;
    public:
        collector() = default;

        void clear() noexcept;

        events::iterator begin() noexcept;
        events::const_iterator begin() const noexcept;
        events::const_iterator cbegin() const noexcept;

        events::iterator end() noexcept;
        events::const_iterator end() const noexcept;
        events::const_iterator cend() const noexcept;
    private:
        void on_move_cursor(
            const v2f& pos) noexcept final;

        void on_mouse_scroll(
            const v2f& delta) noexcept final;

        void on_mouse_button(
            mouse_button button,
            mouse_button_action action) noexcept final;
    private:
        vector<event> events_;
    };
}
