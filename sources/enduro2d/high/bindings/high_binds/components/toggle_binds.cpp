/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/toggle.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace e2d::bindings::high
{
    void bind_toggle(sol::state& l) {
        l.new_usertype<gcomponent<toggle>>("toggle",
            sol::no_constructor,

            "enable", [](gcomponent<toggle>& c){
                c.owner().component<disabled<toggle>>().remove();
            },

            "disable", [](gcomponent<toggle>& c){
                c.owner().component<disabled<toggle>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<toggle>& c) -> bool {
                    return !c.owner().component<disabled<toggle>>().exists();
                },
                [](gcomponent<toggle>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<toggle>>().remove();
                    } else {
                        c.owner().component<disabled<toggle>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<toggle>& c) -> bool {
                    return c.owner().component<disabled<toggle>>().exists();
                },
                [](gcomponent<toggle>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<toggle>>().ensure();
                    } else {
                        c.owner().component<disabled<toggle>>().remove();
                    }
                }
            ),

            "pressed", sol::property(
                [](const gcomponent<toggle>& c) -> bool {
                    return c.owner().component<toggle::pressed>().exists();
                },
                [](gcomponent<toggle>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<toggle::pressed>().ensure();
                    } else {
                        c.owner().component<toggle::pressed>().remove();
                    }
                }
            ),

            "style", sol::property(
                [](const gcomponent<toggle>& c) -> toggle_style_asset::ptr {
                    return c->style();
                },
                [](gcomponent<toggle>& c, const toggle_style_asset::ptr& v){
                    c->style(v);
                })
        );
    }
}
