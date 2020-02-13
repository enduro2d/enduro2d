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
                layouts::mark_dirty(c);
            },

            "disable", [](gcomponent<layout>& c){
                c.owner().component<disabled<layout>>().ensure();
                layouts::mark_dirty(c);
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
                    layouts::mark_dirty(c);
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
                    layouts::mark_dirty(c);
                }
            ),

            "dirty", sol::property(
                [](const gcomponent<layout>& c) -> bool {
                    return layouts::is_dirty(c);
                },
                [](gcomponent<layout>& c, bool yesno){
                    if ( yesno ) {
                        layouts::mark_dirty(c);
                    } else {
                        layouts::unmark_dirty(c);
                    }
                }
            ),

            "mode", sol::property(
                [](const gcomponent<layout>& c) -> layout::modes {
                    return c->mode();
                },
                [](gcomponent<layout>& c, layout::modes v){
                    layouts::change_mode(c, v);
                }),

            "halign", sol::property(
                [](const gcomponent<layout>& c) -> layout::haligns {
                    return c->halign();
                },
                [](gcomponent<layout>& c, layout::haligns v){
                    layouts::change_halign(c, v);
                }),

            "valign", sol::property(
                [](const gcomponent<layout>& c) -> layout::valigns {
                    return c->valign();
                },
                [](gcomponent<layout>& c, layout::valigns v){
                    layouts::change_valign(c, v);
                }),

            "size", sol::property(
                [](const gcomponent<layout>& c) -> v2f {
                    return c->size();
                },
                [](gcomponent<layout>& c, const v2f& v){
                    layouts::change_size(c, v);
                }),

            "margin", sol::property(
                [](const gcomponent<layout>& c) -> v2f {
                    return c->margin();
                },
                [](gcomponent<layout>& c, const v2f& v){
                    layouts::change_margin(c, v);
                }),

            "padding", sol::property(
                [](const gcomponent<layout>& c) -> v2f {
                    return c->padding();
                },
                [](gcomponent<layout>& c, const v2f& v){
                    layouts::change_padding(c, v);
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
            LAYOUT_HALIGN_PAIR(space_around)
            LAYOUT_HALIGN_PAIR(space_evenly)
            LAYOUT_HALIGN_PAIR(space_between)
        });
    #undef LAYOUT_HALIGN_PAIR

    #define LAYOUT_VALIGN_PAIR(x) {#x, layout::valigns::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::valigns>("valigns", {
            LAYOUT_VALIGN_PAIR(top)
            LAYOUT_VALIGN_PAIR(center)
            LAYOUT_VALIGN_PAIR(bottom)
            LAYOUT_VALIGN_PAIR(space_around)
            LAYOUT_VALIGN_PAIR(space_evenly)
            LAYOUT_VALIGN_PAIR(space_between)
        });
    #undef LAYOUT_VALIGN_PAIR
    }
}
