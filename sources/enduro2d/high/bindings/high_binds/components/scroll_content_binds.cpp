/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scroll_content.hpp>

namespace e2d::bindings::high
{
    void bind_scroll_content(sol::state& l) {
        l.new_usertype<gcomponent<scroll_content>>("scroll_content",
            sol::no_constructor,

            "enable", [](gcomponent<scroll_content>& c){
                c.component<disabled<scroll_content>>().remove();
            },

            "disable", [](gcomponent<scroll_content>& c){
                c.component<disabled<scroll_content>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<scroll_content>& c) -> bool {
                    return !c.component<disabled<scroll_content>>().exists();
                },
                [](gcomponent<scroll_content>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scroll_content>>().remove();
                    } else {
                        c.component<disabled<scroll_content>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<scroll_content>& c) -> bool {
                    return c.component<disabled<scroll_content>>().exists();
                },
                [](gcomponent<scroll_content>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scroll_content>>().ensure();
                    } else {
                        c.component<disabled<scroll_content>>().remove();
                    }
                }
            )
        );
    }
}
