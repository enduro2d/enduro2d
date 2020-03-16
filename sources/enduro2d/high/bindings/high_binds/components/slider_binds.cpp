/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/slider.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace e2d::bindings::high
{
    void bind_slider(sol::state& l) {
        l.new_usertype<gcomponent<slider>>("slider",
            sol::no_constructor,

            "enable", [](gcomponent<slider>& c){
                c.owner().component<disabled<slider>>().remove();
            },

            "disable", [](gcomponent<slider>& c){
                c.owner().component<disabled<slider>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<slider>& c) -> bool {
                    return !c.owner().component<disabled<slider>>().exists();
                },
                [](gcomponent<slider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<slider>>().remove();
                    } else {
                        c.owner().component<disabled<slider>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<slider>& c) -> bool {
                    return c.owner().component<disabled<slider>>().exists();
                },
                [](gcomponent<slider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<slider>>().ensure();
                    } else {
                        c.owner().component<disabled<slider>>().remove();
                    }
                }
            ),

            "style", sol::property(
                [](const gcomponent<slider>& c) -> button_style_asset::ptr {
                    return c->style();
                },
                [](gcomponent<slider>& c, const button_style_asset::ptr& v){
                    c->style(v);
                })
        );
    }
}
