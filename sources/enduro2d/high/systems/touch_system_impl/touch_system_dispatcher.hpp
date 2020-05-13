/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "touch_system_base.hpp"

namespace e2d::touch_system_impl
{
    struct hovering_info {
        gobject target;
        gobject camera;
        hovering_info(gobject target, gobject camera)
        : target(std::move(target))
        , camera(std::move(camera)) {}
    };

    struct dragging_info {
        gobject target;
        gobject camera;
        dragging_info(gobject target, gobject camera)
        : target(std::move(target))
        , camera(std::move(camera)) {}
    };

    using hovering_info_opt = std::optional<hovering_info>;
    using dragging_info_opt = std::optional<dragging_info>;

    class dispatcher final {
    public:
        dispatcher() = default;

        void dispatch_all_events(
            collector& collector,
            ecs::registry& owner);
    private:
        hovering_info_opt hovering_;
        dragging_info_opt dragging_;
    };
}
