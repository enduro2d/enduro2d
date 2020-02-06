/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/layout.hpp>

namespace e2d::bindings::high
{
    void bind_layout(sol::state& l) {
        l.new_usertype<gcomponent<layout>>("layout",
            sol::no_constructor,

            "enable", [](gcomponent<layout>& c){
                c.owner().component<disabled<layout>>().remove();
            },

            "disable", [](gcomponent<layout>& c){
                c.owner().component<disabled<layout>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<layout>& c) -> bool {
                    return !c.owner().component<disabled<layout>>().exists();
                },
                [](gcomponent<layout>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<layout>>().remove();
                    } else {
                        c.owner().component<disabled<layout>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<layout>& c) -> bool {
                    return c.owner().component<disabled<layout>>().exists();
                },
                [](gcomponent<layout>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<layout>>().ensure();
                    } else {
                        c.owner().component<disabled<layout>>().remove();
                    }
                }
            ),

            "mode", sol::property(
                [](const gcomponent<layout>& c) -> layout::modes {
                    return c->mode();
                },
                [](gcomponent<layout>& c, layout::modes v){
                    c->mode(v);
                })
        );

    #define LAYOUT_MODE_PAIR(x) {#x, layout::modes::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::modes>("modes", {
            LAYOUT_MODE_PAIR(vertical)
            LAYOUT_MODE_PAIR(horizontal)
        });
    #undef LAYOUT_MODE_PAIR

        l.new_usertype<gcomponent<layout_item>>("layout_item",
            sol::no_constructor,

            "enable", [](gcomponent<layout_item>& c){
                c.owner().component<disabled<layout_item>>().remove();
            },

            "disable", [](gcomponent<layout_item>& c){
                c.owner().component<disabled<layout_item>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<layout_item>& c) -> bool {
                    return !c.owner().component<disabled<layout_item>>().exists();
                },
                [](gcomponent<layout_item>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<layout_item>>().remove();
                    } else {
                        c.owner().component<disabled<layout_item>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<layout_item>& c) -> bool {
                    return c.owner().component<disabled<layout_item>>().exists();
                },
                [](gcomponent<layout_item>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<layout_item>>().ensure();
                    } else {
                        c.owner().component<disabled<layout_item>>().remove();
                    }
                }
            )
        );
    }
}
