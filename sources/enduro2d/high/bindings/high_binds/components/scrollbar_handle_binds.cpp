/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scrollbar_handle.hpp>

namespace e2d::bindings::high
{
    void bind_scrollbar_handle(sol::state& l) {
        l.new_usertype<gcomponent<scrollbar_handle>>("scrollbar_handle",
            sol::no_constructor,

            "enable", [](gcomponent<scrollbar_handle>& c){
                c.component<disabled<scrollbar_handle>>().remove();
            },

            "disable", [](gcomponent<scrollbar_handle>& c){
                c.component<disabled<scrollbar_handle>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<scrollbar_handle>& c) -> bool {
                    return !c.component<disabled<scrollbar_handle>>().exists();
                },
                [](gcomponent<scrollbar_handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scrollbar_handle>>().remove();
                    } else {
                        c.component<disabled<scrollbar_handle>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<scrollbar_handle>& c) -> bool {
                    return c.component<disabled<scrollbar_handle>>().exists();
                },
                [](gcomponent<scrollbar_handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scrollbar_handle>>().ensure();
                    } else {
                        c.component<disabled<scrollbar_handle>>().remove();
                    }
                }
            )
        );
    }
}
