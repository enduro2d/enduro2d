/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/handle.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace e2d::bindings::high
{
    void bind_handle(sol::state& l) {
        l.new_usertype<gcomponent<handle>>("handle",
            sol::no_constructor,

            "enable", [](gcomponent<handle>& c){
                c.component<disabled<handle>>().remove();
            },

            "disable", [](gcomponent<handle>& c){
                c.component<disabled<handle>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<handle>& c) -> bool {
                    return !c.component<disabled<handle>>().exists();
                },
                [](gcomponent<handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<handle>>().remove();
                    } else {
                        c.component<disabled<handle>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<handle>& c) -> bool {
                    return c.component<disabled<handle>>().exists();
                },
                [](gcomponent<handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<handle>>().ensure();
                    } else {
                        c.component<disabled<handle>>().remove();
                    }
                }
            )
        );
    }
}
