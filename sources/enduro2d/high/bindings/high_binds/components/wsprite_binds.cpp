/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/wsprite.hpp>

namespace e2d::bindings::high
{
    void bind_wsprite(sol::state& l) {
        l.new_usertype<gcomponent<wsprite>>("wsprite",
            sol::no_constructor,

            "enable", [](gcomponent<wsprite>& c){
                c.component<disabled<wsprite>>().remove();
            },

            "disable", [](gcomponent<wsprite>& c){
                c.component<disabled<wsprite>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<wsprite>& c) -> bool {
                    return !c.component<disabled<wsprite>>().exists();
                },
                [](gcomponent<wsprite>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<wsprite>>().remove();
                    } else {
                        c.component<disabled<wsprite>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<wsprite>& c) -> bool {
                    return c.component<disabled<wsprite>>().exists();
                },
                [](gcomponent<wsprite>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<wsprite>>().ensure();
                    } else {
                        c.component<disabled<wsprite>>().remove();
                    }
                }
            )
        );
    }
}
