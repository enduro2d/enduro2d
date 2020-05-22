/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scroll.hpp>

namespace e2d::bindings::high
{
    void bind_scroll(sol::state& l) {
        l.new_usertype<gcomponent<scroll>>("scroll",
            sol::no_constructor,

            "enable", [](gcomponent<scroll>& c){
                c.component<disabled<scroll>>().remove();
            },

            "disable", [](gcomponent<scroll>& c){
                c.component<disabled<scroll>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<scroll>& c) -> bool {
                    return !c.component<disabled<scroll>>().exists();
                },
                [](gcomponent<scroll>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scroll>>().remove();
                    } else {
                        c.component<disabled<scroll>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<scroll>& c) -> bool {
                    return c.component<disabled<scroll>>().exists();
                },
                [](gcomponent<scroll>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scroll>>().ensure();
                    } else {
                        c.component<disabled<scroll>>().remove();
                    }
                }
            ),

            "vertical", sol::property(
                [](const gcomponent<scroll>& c) -> bool {
                    return c->vertical();
                },
                [](gcomponent<scroll>& c, bool v){
                    c->vertical(v);
                }),

            "horizontal", sol::property(
                [](const gcomponent<scroll>& c) -> bool {
                    return c->horizontal();
                },
                [](gcomponent<scroll>& c, bool v){
                    c->horizontal(v);
                }),

            "inertia", sol::property(
                [](const gcomponent<scroll>& c) -> f32 {
                    return c->inertia();
                },
                [](gcomponent<scroll>& c, f32 v){
                    c->inertia(v);
                }),

            "elasticity", sol::property(
                [](const gcomponent<scroll>& c) -> f32 {
                    return c->elasticity();
                },
                [](gcomponent<scroll>& c, f32 v){
                    c->elasticity(v);
                }),

            "sensitivity", sol::property(
                [](const gcomponent<scroll>& c) -> f32 {
                    return c->sensitivity();
                },
                [](gcomponent<scroll>& c, f32 v){
                    c->sensitivity(v);
                })
        );
    }
}
