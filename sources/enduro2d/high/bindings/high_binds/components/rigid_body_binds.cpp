/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/rigid_body.hpp>

namespace e2d::bindings::high
{
    void bind_rigid_body(sol::state& l) {
        l.new_usertype<gcomponent<rigid_body>>("rigid_body",
            sol::no_constructor,

            "enable", [](gcomponent<rigid_body>& c){
                c.owner().component<disabled<rigid_body>>().remove();
            },

            "disable", [](gcomponent<rigid_body>& c){
                c.owner().component<disabled<rigid_body>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<rigid_body>& c) -> bool {
                    return !c.owner().component<disabled<rigid_body>>().exists();
                },
                [](gcomponent<rigid_body>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<rigid_body>>().remove();
                    } else {
                        c.owner().component<disabled<rigid_body>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<rigid_body>& c) -> bool {
                    return c.owner().component<disabled<rigid_body>>().exists();
                },
                [](gcomponent<rigid_body>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<rigid_body>>().ensure();
                    } else {
                        c.owner().component<disabled<rigid_body>>().remove();
                    }
                }
            )
        );
    }
}
