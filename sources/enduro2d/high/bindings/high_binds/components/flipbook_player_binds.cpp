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
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<flipbook_player>>("flipbook_player",
            "time", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->time(); },
                [](gcomponent<flipbook_player>& b, f32 v){ b->time(v); }),
            "speed", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->speed(); },
                [](gcomponent<flipbook_player>& b, f32 v){ b->speed(v); }),
            "looped", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->looped(); },
                [](gcomponent<flipbook_player>& b, bool v){ b->looped(v); }),
            "stopped", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->stopped(); },
                [](gcomponent<flipbook_player>& b, bool v){ b->stopped(v); }),
            "playing", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->playing(); },
                [](gcomponent<flipbook_player>& b, bool v){ b->playing(v); }),
            "sequence", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->sequence(); },
                [](gcomponent<flipbook_player>& b, str_hash v){ b->sequence(v); }),
            "flipbook", sol::property(
                [](const gcomponent<flipbook_player>& b){ return b->flipbook(); },
                [](gcomponent<flipbook_player>& b, const flipbook_asset::ptr& v){ b->flipbook(v); }),

            "stop", sol::overload(
                [](gcomponent<flipbook_player>& b, f32 v){ b->stop(v); },
                [](gcomponent<flipbook_player>& b, str_hash v){ b->stop(v); }),
            "play", sol::overload(
                [](gcomponent<flipbook_player>& b, f32 v){ b->play(v); },
                [](gcomponent<flipbook_player>& b, str_hash v){ b->play(v); })
        );
    }
}
