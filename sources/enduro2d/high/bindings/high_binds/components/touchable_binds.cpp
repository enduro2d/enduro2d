/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace e2d::bindings::high
{
    void bind_touchable(sol::state& l) {
        l.new_usertype<gcomponent<touchable>>("touchable",
            sol::no_constructor,

            "enable", [](gcomponent<touchable>& c){
                c.owner().component<disabled<touchable>>().remove();
            },

            "disable", [](gcomponent<touchable>& c){
                c.owner().component<disabled<touchable>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return !c.owner().component<disabled<touchable>>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<touchable>>().remove();
                    } else {
                        c.owner().component<disabled<touchable>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.owner().component<disabled<touchable>>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<touchable>>().ensure();
                    } else {
                        c.owner().component<disabled<touchable>>().remove();
                    }
                }
            )
        );
    }
}
