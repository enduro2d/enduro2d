/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
                c.component<disabled<behaviour>>().remove();
            },

            "disable", [](gcomponent<behaviour>& c){
                c.component<disabled<behaviour>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<behaviour>& c) -> bool {
                    return !c.component<disabled<behaviour>>().exists();
                },
                [](gcomponent<behaviour>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<behaviour>>().remove();
                    } else {
                        c.component<disabled<behaviour>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<behaviour>& c) -> bool {
                    return c.component<disabled<behaviour>>().exists();
                },
                [](gcomponent<behaviour>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<behaviour>>().ensure();
                    } else {
                        c.component<disabled<behaviour>>().remove();
                    }
                }
            ),

            "meta", sol::property([](const gcomponent<behaviour>& c) -> sol::table {
                return c->meta();
            })
        );
    }
}
