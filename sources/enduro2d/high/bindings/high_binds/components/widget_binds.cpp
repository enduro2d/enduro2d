/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace e2d::bindings::high
{
    void bind_widget(sol::state& l) {
        l.new_usertype<gcomponent<widget>>("widget",
            sol::no_constructor,

            "enable", [](gcomponent<widget>& c){
                c.owner().component<disabled<widget>>().remove();
            },

            "disable", [](gcomponent<widget>& c){
                c.owner().component<disabled<widget>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<widget>& c) -> bool {
                    return !c.owner().component<disabled<widget>>().exists();
                },
                [](gcomponent<widget>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<widget>>().remove();
                    } else {
                        c.owner().component<disabled<widget>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<widget>& c) -> bool {
                    return c.owner().component<disabled<widget>>().exists();
                },
                [](gcomponent<widget>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<widget>>().ensure();
                    } else {
                        c.owner().component<disabled<widget>>().remove();
                    }
                }
            ),

            "size", sol::property(
                [](const gcomponent<widget>& c) -> v2f {
                    return c->size();
                },
                [](gcomponent<widget>& c, const v2f& v){
                    c->size(v);
                }),

            "pivot", sol::property(
                [](const gcomponent<widget>& c) -> v2f {
                    return c->pivot();
                },
                [](gcomponent<widget>& c, const v2f& v){
                    c->pivot(v);
                })
        );
    }
}
