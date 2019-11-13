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
        l.new_usertype<gcomponent<sprite_renderer>>("sprite_renderer",
            sol::no_constructor,

            "tint", sol::property(
                [](const gcomponent<sprite_renderer>& c) -> color32 {
                    return c->tint();
                },
                [](gcomponent<sprite_renderer>& c, const color32& v){
                    c->tint(v);
                }),

            "blending", sol::property(
                [](const gcomponent<sprite_renderer>& c) -> sprite_renderer::blendings {
                    return c->blending();
                },
                [](gcomponent<sprite_renderer>& c, sprite_renderer::blendings v){
                    c->blending(v);
                }),

            "filtering", sol::property(
                [](const gcomponent<sprite_renderer>& c) -> bool {
                    return c->filtering();
                },
                [](gcomponent<sprite_renderer>& c, bool v){
                    c->filtering(v);
                }),

            "sprite", sol::property(
                [](const gcomponent<sprite_renderer>& c) -> sprite_asset::ptr {
                    return c->sprite();
                },
                [](gcomponent<sprite_renderer>& c, const sprite_asset::ptr& v){
                    c->sprite(v);
                })
        );

    #define SPRITE_RENDERER_BLENDINGS_PAIR(x) {#x, sprite_renderer::blendings::x},
        l["sprite_renderer"].get_or_create<sol::table>()
        .new_enum<sprite_renderer::blendings>("blendings", {
            SPRITE_RENDERER_BLENDINGS_PAIR(normal)
            SPRITE_RENDERER_BLENDINGS_PAIR(additive)
            SPRITE_RENDERER_BLENDINGS_PAIR(multiply)
            SPRITE_RENDERER_BLENDINGS_PAIR(screen)
        });
    #undef SPRITE_RENDERER_BLENDINGS_PAIR
    }
}
