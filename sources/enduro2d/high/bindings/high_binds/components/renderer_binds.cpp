/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/renderer.hpp>

namespace e2d::bindings::high
{
    void bind_renderer(sol::state& l) {
        l.new_usertype<gcomponent<renderer>>("renderer",
            sol::no_constructor,

            "enable", [](gcomponent<renderer>& c){
                c.owner().component<disabled<renderer>>().remove();
            },

            "disable", [](gcomponent<renderer>& c){
                c.owner().component<disabled<renderer>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<renderer>& c) -> bool {
                    return !c.owner().component<disabled<renderer>>().exists();
                },
                [](gcomponent<renderer>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<renderer>>().remove();
                    } else {
                        c.owner().component<disabled<renderer>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<renderer>& c) -> bool {
                    return c.owner().component<disabled<renderer>>().exists();
                },
                [](gcomponent<renderer>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<renderer>>().ensure();
                    } else {
                        c.owner().component<disabled<renderer>>().remove();
                    }
                }
            ),

            "transform", sol::property(
                [](const gcomponent<renderer>& c) -> t3f { return c->transform(); },
                [](gcomponent<renderer>& c, const t3f& v) { c->transform(v); }),

            "translation", sol::property(
                [](const gcomponent<renderer>& c) -> v3f { return c->translation(); },
                [](gcomponent<renderer>& c, const v3f& v) { c->translation(v); }),

            "rotation", sol::property(
                [](const gcomponent<renderer>& c) -> q4f { return c->rotation(); },
                [](gcomponent<renderer>& c, const q4f& v) { c->rotation(v); }),

            "scale", sol::property(
                [](const gcomponent<renderer>& c) -> v3f { return c->scale(); },
                [](gcomponent<renderer>& c, const v3f& v) { c->scale(v); })
        );
    }
}
