/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/progress_handle.hpp>

namespace e2d::bindings::high
{
    void bind_progress_handle(sol::state& l) {
        l.new_usertype<gcomponent<progress_handle>>("progress_handle",
            sol::no_constructor,

            "enable", [](gcomponent<progress_handle>& c){
                c.component<disabled<progress_handle>>().remove();
            },

            "disable", [](gcomponent<progress_handle>& c){
                c.component<disabled<progress_handle>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<progress_handle>& c) -> bool {
                    return !c.component<disabled<progress_handle>>().exists();
                },
                [](gcomponent<progress_handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<progress_handle>>().remove();
                    } else {
                        c.component<disabled<progress_handle>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<progress_handle>& c) -> bool {
                    return c.component<disabled<progress_handle>>().exists();
                },
                [](gcomponent<progress_handle>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<progress_handle>>().ensure();
                    } else {
                        c.component<disabled<progress_handle>>().remove();
                    }
                }
            )
        );
    }
}
