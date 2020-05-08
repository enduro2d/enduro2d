/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/_high.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/colliders.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/events.hpp>
#include <enduro2d/high/components/scene.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace e2d::touch_system_impl
{
    class collector;
    class dispatcher;
}

namespace e2d::touch_system_impl
{
    struct touchable_under_mouse final {
        v2f local_point{v2f::zero()};
        v2f world_point{v2f::zero()};
    };

    struct world_space_collider_base {
        m4f local_to_world{m4f::identity()};
    };

    struct world_space_rect_collider final : world_space_collider_base {
        using local_space_collider_t = rect_collider;
        std::array<v3f, 4> points{};
    };

    struct world_space_circle_collider final : world_space_collider_base {
        using local_space_collider_t = circle_collider;
        std::array<v3f, 12> points{};
    };

    struct world_space_polygon_collider final : world_space_collider_base {
        using local_space_collider_t = polygon_collider;
        vector<v3f> points{};
    };
}
