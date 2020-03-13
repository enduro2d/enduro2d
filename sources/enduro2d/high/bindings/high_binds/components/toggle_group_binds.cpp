/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/toggle_group.hpp>

namespace e2d::bindings::high
{
    void bind_toggle_group(sol::state& l) {
        l.new_usertype<gcomponent<toggle_group>>("toggle_group",
            sol::no_constructor,

            "enable", [](gcomponent<toggle_group>& c){
                c.owner().component<disabled<toggle_group>>().remove();
            },

            "disable", [](gcomponent<toggle_group>& c){
                c.owner().component<disabled<toggle_group>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<toggle_group>& c) -> bool {
                    return !c.owner().component<disabled<toggle_group>>().exists();
                },
                [](gcomponent<toggle_group>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<toggle_group>>().remove();
                    } else {
                        c.owner().component<disabled<toggle_group>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<toggle_group>& c) -> bool {
                    return c.owner().component<disabled<toggle_group>>().exists();
                },
                [](gcomponent<toggle_group>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<toggle_group>>().ensure();
                    } else {
                        c.owner().component<disabled<toggle_group>>().remove();
                    }
                }
            ),

            "allow_switch_off", sol::property(
                [](const gcomponent<toggle_group>& c) -> bool {
                    return c->allow_switch_off();
                },
                [](gcomponent<toggle_group>& c, bool v){
                    c->allow_switch_off(v);
                }),

            "allow_multiple_on", sol::property(
                [](const gcomponent<toggle_group>& c) -> bool {
                    return c->allow_multiple_on();
                },
                [](gcomponent<toggle_group>& c, bool v){
                    c->allow_multiple_on(v);
                })
        );
    }
}
