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
                c.component<disabled<layout>>().remove();
                layouts::mark_dirty(c);
            },

            "disable", [](gcomponent<layout>& c){
                c.component<disabled<layout>>().ensure();
                layouts::mark_dirty(c);
            },

            "enabled", sol::property(
                [](const gcomponent<layout>& c) -> bool {
                    return !c.component<disabled<layout>>().exists();
                },
                [](gcomponent<layout>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<layout>>().remove();
                    } else {
                        c.component<disabled<layout>>().ensure();
                    }
                    layouts::mark_dirty(c);
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<layout>& c) -> bool {
                    return c.component<disabled<layout>>().exists();
                },
                [](gcomponent<layout>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<layout>>().ensure();
                    } else {
                        c.component<disabled<layout>>().remove();
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

            "direction", sol::property(
                [](const gcomponent<layout>& c) -> layout::directions {
                    return c->direction();
                },
                [](gcomponent<layout>& c, layout::directions v){
                    layouts::change_direction(c, v);
                }),

            "align_items", sol::property(
                [](const gcomponent<layout>& c) -> layout::align_modes {
                    return c->align_items();
                },
                [](gcomponent<layout>& c, layout::align_modes v){
                    layouts::change_align_items(c, v);
                }),

            "align_content", sol::property(
                [](const gcomponent<layout>& c) -> layout::align_modes {
                    return c->align_content();
                },
                [](gcomponent<layout>& c, layout::align_modes v){
                    layouts::change_align_content(c, v);
                }),

            "justify_content", sol::property(
                [](const gcomponent<layout>& c) -> layout::justify_modes {
                    return c->justify_content();
                },
                [](gcomponent<layout>& c, layout::justify_modes v){
                    layouts::change_justify_content(c, v);
                }),

            "flex_wrap", sol::property(
                [](const gcomponent<layout>& c) -> layout::flex_wraps {
                    return c->flex_wrap();
                },
                [](gcomponent<layout>& c, layout::flex_wraps v){
                    layouts::change_flex_wrap(c, v);
                }),

            "flex_direction", sol::property(
                [](const gcomponent<layout>& c) -> layout::flex_directions {
                    return c->flex_direction();
                },
                [](gcomponent<layout>& c, layout::flex_directions v){
                    layouts::change_flex_direction(c, v);
                })
        );

    #define LAYOUT_DIRECTION_PAIR(x) {#x, layout::directions::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::directions>("directions", {
            LAYOUT_DIRECTION_PAIR(ltr)
            LAYOUT_DIRECTION_PAIR(rtl)
        });
    #undef LAYOUT_DIRECTION_PAIR

    #define LAYOUT_ALIGN_MODE_PAIR(x) {#x, layout::align_modes::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::align_modes>("align_modes", {
            LAYOUT_ALIGN_MODE_PAIR(flex_start)
            LAYOUT_ALIGN_MODE_PAIR(center)
            LAYOUT_ALIGN_MODE_PAIR(flex_end)
            LAYOUT_ALIGN_MODE_PAIR(space_between)
            LAYOUT_ALIGN_MODE_PAIR(space_around)
        });
    #undef LAYOUT_ALIGN_MODE_PAIR

    #define LAYOUT_JUSTIFY_MODE_PAIR(x) {#x, layout::justify_modes::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::justify_modes>("justify_modes", {
            LAYOUT_JUSTIFY_MODE_PAIR(flex_start)
            LAYOUT_JUSTIFY_MODE_PAIR(center)
            LAYOUT_JUSTIFY_MODE_PAIR(flex_end)
            LAYOUT_JUSTIFY_MODE_PAIR(space_between)
            LAYOUT_JUSTIFY_MODE_PAIR(space_around)
            LAYOUT_JUSTIFY_MODE_PAIR(space_evenly)
        });
    #undef LAYOUT_JUSTIFY_MODE_PAIR

    #define LAYOUT_FLEX_WRAP_PAIR(x) {#x, layout::flex_wraps::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::flex_wraps>("flex_wraps", {
            LAYOUT_FLEX_WRAP_PAIR(no_wrap)
            LAYOUT_FLEX_WRAP_PAIR(wrap)
            LAYOUT_FLEX_WRAP_PAIR(wrap_reversed)
        });
    #undef LAYOUT_FLEX_WRAP_PAIR

    #define LAYOUT_FLEX_DIRECTION_PAIR(x) {#x, layout::flex_directions::x},
        l["layout"].get_or_create<sol::table>()
        .new_enum<layout::flex_directions>("flex_directions", {
            LAYOUT_FLEX_DIRECTION_PAIR(row)
            LAYOUT_FLEX_DIRECTION_PAIR(row_reversed)
            LAYOUT_FLEX_DIRECTION_PAIR(column)
            LAYOUT_FLEX_DIRECTION_PAIR(column_reversed)
        });
    #undef LAYOUT_FLEX_DIRECTION_PAIR
    }
}
