/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scrollbar.hpp>

namespace e2d::bindings::high
{
    void bind_scrollbar(sol::state& l) {
        l.new_usertype<gcomponent<scrollbar>>("scrollbar",
            sol::no_constructor,

            "enable", [](gcomponent<scrollbar>& c){
                c.component<disabled<scrollbar>>().remove();
            },

            "disable", [](gcomponent<scrollbar>& c){
                c.component<disabled<scrollbar>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<scrollbar>& c) -> bool {
                    return !c.component<disabled<scrollbar>>().exists();
                },
                [](gcomponent<scrollbar>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scrollbar>>().remove();
                    } else {
                        c.component<disabled<scrollbar>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<scrollbar>& c) -> bool {
                    return c.component<disabled<scrollbar>>().exists();
                },
                [](gcomponent<scrollbar>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<scrollbar>>().ensure();
                    } else {
                        c.component<disabled<scrollbar>>().remove();
                    }
                }
            ),

            "value", sol::property(
                [](const gcomponent<scrollbar>& c) -> f32 {
                    return c->value();
                },
                [](gcomponent<scrollbar>& c, f32 v){
                    c->value(v);
                }),

            "direction", sol::property(
                [](const gcomponent<scrollbar>& c) -> scrollbar::directions {
                    return c->direction();
                },
                [](gcomponent<scrollbar>& c, scrollbar::directions v){
                    c->direction(v);
                })
        );
    }
}
