/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/camera.hpp>

namespace e2d::bindings::high
{
    void bind_camera(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<camera>>("camera",
            "depth", sol::property(
                [](const gcomponent<camera>& b){ return b->depth(); },
                [](gcomponent<camera>& b, i32 v){ b->depth(v); }),
            "viewport", sol::property(
                [](const gcomponent<camera>& b){ return b->viewport().cast_to<f32>(); },
                [](gcomponent<camera>& b, const b2f& v){ b->viewport(v.cast_to<u32>()); }),
            "projection", sol::property(
                [](const gcomponent<camera>& b){ return b->projection(); },
                [](gcomponent<camera>& b, const m4f& v){ b->projection(v); }),
            "target", sol::property(
                [](const gcomponent<camera>& b){ return b->target(); },
                [](gcomponent<camera>& b, const render_target_ptr& v){ b->target(v); }),
            "background", sol::property(
                [](const gcomponent<camera>& b){ return b->background(); },
                [](gcomponent<camera>& b, const color& v){ b->background(v); })
        );
    }
}
