/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/slider_handle.hpp>

namespace e2d::bindings::high
{
    void bind_slider_handle(sol::state& l) {
        l.new_usertype<gcomponent<slider_handle>>("slider_handle",
            sol::no_constructor,

            "enable", [](gcomponent<slider_handle>& c){
                c.component<disabled<slider_handle>>().remove();
            },

            "disable", [](gcomponent<slider_handle>& c){
                c.component<disabled<slider_handle>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<slider_handle>& c) -> bool {
                    return !c.component<disabled<slider_handle>>().exists();
                },
                [](gcomponent<slider_handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<slider_handle>>().remove();
                    } else {
                        c.component<disabled<slider_handle>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<slider_handle>& c) -> bool {
                    return c.component<disabled<slider_handle>>().exists();
                },
                [](gcomponent<slider_handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<slider_handle>>().ensure();
                    } else {
                        c.component<disabled<slider_handle>>().remove();
                    }
                }
            )
        );
    }
}
