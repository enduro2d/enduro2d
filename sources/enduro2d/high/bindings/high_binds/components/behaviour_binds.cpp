/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace e2d::bindings::high
{
    void bind_behaviour(sol::state& l) {
        l.new_usertype<gcomponent<behaviour>>("behaviour",
            sol::no_constructor,

            "enable", [](gcomponent<behaviour>& c){
                c.owner().component<disabled<behaviour>>().remove();
            },

            "disable", [](gcomponent<behaviour>& c){
                c.owner().component<disabled<behaviour>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<behaviour>& c) -> bool {
                    return !c.owner().component<disabled<behaviour>>().exists();
                },
                [](gcomponent<behaviour>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<behaviour>>().remove();
                    } else {
                        c.owner().component<disabled<behaviour>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<behaviour>& c) -> bool {
                    return c.owner().component<disabled<behaviour>>().exists();
                },
                [](gcomponent<behaviour>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<behaviour>>().ensure();
                    } else {
                        c.owner().component<disabled<behaviour>>().remove();
                    }
                }
            ),

            "meta", sol::property([](const gcomponent<behaviour>& c) -> sol::table {
                return c->meta();
            })
        );
    }
}
