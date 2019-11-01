/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

namespace e2d::bindings::high
{
    void bind_sprite_renderer(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<sprite_renderer>>("sprite_renderer",
            "tint", sol::property(
                [](const gcomponent<sprite_renderer>& l){ return l->tint(); },
                [](gcomponent<sprite_renderer>& l, const color32& v){ l->tint(v); }),
            "blending", sol::property(
                [](const gcomponent<sprite_renderer>& l){ return l->blending(); },
                [](gcomponent<sprite_renderer>& l, sprite_renderer::blendings v){ l->blending(v); }),
            "filtering", sol::property(
                [](const gcomponent<sprite_renderer>& l){ return l->filtering(); },
                [](gcomponent<sprite_renderer>& l, bool v){ l->filtering(v); }),
            "sprite", sol::property(
                [](const gcomponent<sprite_renderer>& l){ return l->sprite(); },
                [](gcomponent<sprite_renderer>& l, const sprite_asset::ptr& v){ l->sprite(v); })
        );

    #define SPRITE_RENDERER_BLENDINGS_PAIR(x) {#x, sprite_renderer::blendings::x},
        l["e2d"].get_or_create<sol::table>()
        ["sprite_renderer"].get_or_create<sol::table>()
        .new_enum<sprite_renderer::blendings>("blendings", {
            SPRITE_RENDERER_BLENDINGS_PAIR(normal)
            SPRITE_RENDERER_BLENDINGS_PAIR(additive)
            SPRITE_RENDERER_BLENDINGS_PAIR(multiply)
            SPRITE_RENDERER_BLENDINGS_PAIR(screen)
        });
    #undef SPRITE_RENDERER_BLENDINGS_PAIR
    }
}
