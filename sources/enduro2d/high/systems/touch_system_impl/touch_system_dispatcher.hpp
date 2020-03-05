/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "touch_system_base.hpp"

namespace e2d::touch_system_impl
{
    class dispatcher final : public window::event_listener {
    public:
        dispatcher() = default;

        void dispatch_all_events(ecs::registry& owner);
    private:
        void on_mouse_button(
            mouse_button button,
            mouse_button_action action) noexcept final;
    private:
        gobject last_hover_target_;
        vector<touchable_events::event> events_;
    };
}
