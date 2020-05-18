/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/progress.hpp>

namespace e2d::bindings::high
{
    void bind_progress(sol::state& l) {
        l.new_usertype<gcomponent<progress>>("progress",
            sol::no_constructor,

            "enable", [](gcomponent<progress>& c){
                c.component<disabled<progress>>().remove();
            },

            "disable", [](gcomponent<progress>& c){
                c.component<disabled<progress>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<progress>& c) -> bool {
                    return !c.component<disabled<progress>>().exists();
                },
                [](gcomponent<progress>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<progress>>().remove();
                    } else {
                        c.component<disabled<progress>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<progress>& c) -> bool {
                    return c.component<disabled<progress>>().exists();
                },
                [](gcomponent<progress>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<progress>>().ensure();
                    } else {
                        c.component<disabled<progress>>().remove();
                    }
                }
            ),

            "value", sol::property(
                [](const gcomponent<progress>& c) -> f32 {
                    return c->value();
                },
                [](gcomponent<progress>& c, f32 v){
                    c->value(v);
                }),

            "direction", sol::property(
                [](const gcomponent<progress>& c) -> progress::directions {
                    return c->direction();
                },
                [](gcomponent<progress>& c, progress::directions v){
                    c->direction(v);
                })
        );
    }
}
