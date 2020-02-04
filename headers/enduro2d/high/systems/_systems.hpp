/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d::systems
{
    struct update_event {
        f32 dt{0.f};
        f32 time{0.f};
    };

    struct pre_update_event {
        f32 dt{0.f};
        f32 time{0.f};
    };

    struct post_update_event {
        f32 dt{0.f};
        f32 time{0.f};
    };

    struct render_event {
        ecs::const_entity cam_e;
    };

    struct pre_render_event {
        ecs::const_entity cam_e;
    };

    struct post_render_event {
        ecs::const_entity cam_e;
    };

    struct frame_update_event {};
    struct frame_render_event {};
    struct frame_finalize_event {};

    using update_system = ecs::system<update_event>;
    using post_update_system = ecs::system<post_update_event>;

    using render_system = ecs::system<render_event>;
    using post_render_system = ecs::system<post_render_event>;
}
