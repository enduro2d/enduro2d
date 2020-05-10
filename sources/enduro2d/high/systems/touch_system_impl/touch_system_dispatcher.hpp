/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "touch_system_base.hpp"

namespace e2d::touch_system_impl
{
    class dispatcher final {
    public:
        dispatcher() = default;

        void dispatch_all_events(
            collector& collector,
            ecs::registry& owner);
    private:
        gobject last_target_;
    };
}
