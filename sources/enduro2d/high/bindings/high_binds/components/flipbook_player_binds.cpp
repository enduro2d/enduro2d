/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/flipbook_player.hpp>

namespace e2d::bindings::high
{
    void bind_flipbook_player(sol::state& l) {
        l.new_usertype<gcomponent<flipbook_player>>("flipbook_player",
            sol::no_constructor,

            "time", sol::property(
                [](const gcomponent<flipbook_player>& c) -> f32 {
                    return c->time();
                },
                [](gcomponent<flipbook_player>& c, f32 v){
                    c->time(v);
                }),

            "speed", sol::property(
                [](const gcomponent<flipbook_player>& c) -> f32 {
                    return c->speed();
                },
                [](gcomponent<flipbook_player>& c, f32 v){
                    c->speed(v);
                }),

            "looped", sol::property(
                [](const gcomponent<flipbook_player>& c) -> bool {
                    return c->looped();
                },
                [](gcomponent<flipbook_player>& c, bool v){
                    c->looped(v);
                }),

            "stopped", sol::property(
                [](const gcomponent<flipbook_player>& c) -> bool {
                    return c->stopped();
                },
                [](gcomponent<flipbook_player>& c, bool v){
                    c->stopped(v);
                }),

            "playing", sol::property(
                [](const gcomponent<flipbook_player>& c) -> bool {
                    return c->playing();
                },
                [](gcomponent<flipbook_player>& c, bool v) {
                    c->playing(v);
                }),

            "sequence", sol::property(
                [](const gcomponent<flipbook_player>& c) -> str_hash {
                    return c->sequence();
                },
                [](gcomponent<flipbook_player>& c, str_hash v){
                    c->sequence(v);
                }),

            "flipbook", sol::property(
                [](const gcomponent<flipbook_player>& c) -> flipbook_asset::ptr {
                    return c->flipbook();
                },
                [](gcomponent<flipbook_player>& c, const flipbook_asset::ptr& v){
                    c->flipbook(v);
                }),

            "stop", sol::overload(
                [](gcomponent<flipbook_player>& c, f32 v){ c->stop(v); },
                [](gcomponent<flipbook_player>& c, str_view v){ c->stop(v); },
                [](gcomponent<flipbook_player>& c, str_hash v){ c->stop(v); }),

            "play", sol::overload(
                [](gcomponent<flipbook_player>& c, f32 v){ c->play(v); },
                [](gcomponent<flipbook_player>& c, str_view v){ c->play(v); },
                [](gcomponent<flipbook_player>& c, str_hash v){ c->play(v); })
        );
    }
}
