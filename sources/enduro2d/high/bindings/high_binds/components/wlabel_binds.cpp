/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/wlabel.hpp>

namespace e2d::bindings::high
{
    void bind_wlabel(sol::state& l) {
        l.new_usertype<gcomponent<wlabel>>("wlabel",
            sol::no_constructor,

            "enable", [](gcomponent<wlabel>& c){
                c.component<disabled<wlabel>>().remove();
            },

            "disable", [](gcomponent<wlabel>& c){
                c.component<disabled<wlabel>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<wlabel>& c) -> bool {
                    return !c.component<disabled<wlabel>>().exists();
                },
                [](gcomponent<wlabel>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<wlabel>>().remove();
                    } else {
                        c.component<disabled<wlabel>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<wlabel>& c) -> bool {
                    return c.component<disabled<wlabel>>().exists();
                },
                [](gcomponent<wlabel>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<wlabel>>().ensure();
                    } else {
                        c.component<disabled<wlabel>>().remove();
                    }
                }
            )
        );
    }
}
