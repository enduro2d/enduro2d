/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "touch_system_collector.hpp"

namespace e2d::touch_system_impl
{
    void collector::clear() noexcept {
        events_.clear();
    }

    collector::events::iterator collector::begin() noexcept {
        return events_.begin();
    }

    collector::events::const_iterator collector::begin() const noexcept {
        return events_.begin();
    }

    collector::events::const_iterator collector::cbegin() const noexcept {
        return events_.cbegin();
    }

    collector::events::iterator collector::end() noexcept {
        return events_.end();
    }

    collector::events::const_iterator collector::end() const noexcept {
        return events_.end();
    }

    collector::events::const_iterator collector::cend() const noexcept {
        return events_.cend();
    }

    void collector::on_move_cursor(const v2f& pos) noexcept {
        events_.push_back(mouse_move_event(pos));
    }

    void collector::on_mouse_scroll(const v2f& delta) noexcept {
        events_.push_back(mouse_scroll_event(delta));
    }

    void collector::on_mouse_button(mouse_button button, mouse_button_action action) noexcept {
        if ( button == mouse_button::unknown ) {
            return;
        }

        if ( action == mouse_button_action::unknown ) {
            return;
        }

        events_.push_back(mouse_button_event(button, action));
    }
}