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
                }),

            "valign", sol::property(
                [](const gcomponent<layout>& c) -> layout::valigns {
                    return c->valign();
                },
                [](gcomponent<layout>& c, layout::valigns v){
                    c->valign(v);
                }),

            "halign", sol::property(
                [](const gcomponent<layout>& c) -> layout::haligns {
                    return c->halign();
                },
                [](gcomponent<layout>& c, layout::haligns v){
                    c->halign(v);
                }),

            "spacing", sol::property(
                [](const gcomponent<layout>& c) -> f32 {
                    return c->spacing();
                },
                [](gcomponent<layout>& c, f32 v){
                    c->spacing(v);
                })
        );

    #define LAYOUT_MODE_PAIR(x) {#x, layout::modes::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::modes>("modes", {
            LAYOUT_MODE_PAIR(vertical)
            LAYOUT_MODE_PAIR(horizontal)
        });
    #undef LAYOUT_MODE_PAIR

    #define LAYOUT_HALIGN_PAIR(x) {#x, layout::haligns::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::haligns>("haligns", {
            LAYOUT_HALIGN_PAIR(left)
            LAYOUT_HALIGN_PAIR(center)
            LAYOUT_HALIGN_PAIR(right)
        });
    #undef LAYOUT_HALIGN_PAIR

    #define LAYOUT_VALIGN_PAIR(x) {#x, layout::valigns::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::valigns>("valigns", {
            LAYOUT_VALIGN_PAIR(top)
            LAYOUT_VALIGN_PAIR(center)
            LAYOUT_VALIGN_PAIR(bottom)
        });
    #undef LAYOUT_VALIGN_PAIR

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
            ),

            "size", sol::property(
                [](const gcomponent<layout_item>& c) -> v2f {
                    return c->size();
                },
                [](gcomponent<layout_item>& c, const v2f& v){
                    c->size(v);
                })
        );
    }
}
