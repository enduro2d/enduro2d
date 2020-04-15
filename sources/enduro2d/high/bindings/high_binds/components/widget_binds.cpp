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
                c.component<disabled<widget>>().remove();
            },

            "disable", [](gcomponent<widget>& c){
                c.component<disabled<widget>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<widget>& c) -> bool {
                    return !c.component<disabled<widget>>().exists();
                },
                [](gcomponent<widget>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<widget>>().remove();
                    } else {
                        c.component<disabled<widget>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<widget>& c) -> bool {
                    return c.component<disabled<widget>>().exists();
                },
                [](gcomponent<widget>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<widget>>().ensure();
                    } else {
                        c.component<disabled<widget>>().remove();
                    }
                }
            ),

            "dirty", sol::property(
                [](const gcomponent<widget>& c) -> bool {
                    return widgets::is_dirty(c);
                },
                [](gcomponent<widget>& c, bool yesno){
                    if ( yesno ) {
                        widgets::mark_dirty(c);
                    } else {
                        widgets::unmark_dirty(c);
                    }
                }
            ),

            "size", sol::property(
                [](const gcomponent<widget>& c) -> v2f {
                    return c->size();
                },
                [](gcomponent<widget>& c, const v2f& v){
                    widgets::change_size(c, v);
                }),

            "margin", sol::property(
                [](const gcomponent<widget>& c) -> v2f {
                    return c->margin();
                },
                [](gcomponent<widget>& c, const v2f& v){
                    widgets::change_margin(c, v);
                }),

            "padding", sol::property(
                [](const gcomponent<widget>& c) -> v2f {
                    return c->padding();
                },
                [](gcomponent<widget>& c, const v2f& v){
                    widgets::change_padding(c, v);
                })
        );
    }
}
