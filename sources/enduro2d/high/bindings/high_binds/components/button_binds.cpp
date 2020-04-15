/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/button.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace e2d::bindings::high
{
    void bind_button(sol::state& l) {
        l.new_usertype<gcomponent<button>>("button",
            sol::no_constructor,

            "enable", [](gcomponent<button>& c){
                c.component<disabled<button>>().remove();
            },

            "disable", [](gcomponent<button>& c){
                c.component<disabled<button>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<button>& c) -> bool {
                    return !c.component<disabled<button>>().exists();
                },
                [](gcomponent<button>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<button>>().remove();
                    } else {
                        c.component<disabled<button>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<button>& c) -> bool {
                    return c.component<disabled<button>>().exists();
                },
                [](gcomponent<button>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<button>>().ensure();
                    } else {
                        c.component<disabled<button>>().remove();
                    }
                }
            ),

            "pressed", sol::property(
                [](const gcomponent<button>& c) -> bool {
                    return c.component<button::pressed>().exists();
                },
                [](gcomponent<button>& c, bool yesno){
                    if ( yesno ) {
                        c.component<button::pressed>().ensure();
                    } else {
                        c.component<button::pressed>().remove();
                    }
                }
            ),

            "style", sol::property(
                [](const gcomponent<button>& c) -> button_style_asset::ptr {
                    return c->style();
                },
                [](gcomponent<button>& c, const button_style_asset::ptr& v){
                    c->style(v);
                })
        );
    }
}
