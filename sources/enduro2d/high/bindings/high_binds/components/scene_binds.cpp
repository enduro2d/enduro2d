/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scene.hpp>

namespace e2d::bindings::high
{
    void bind_scene(sol::state& l) {
        l.new_usertype<gcomponent<scene>>("scene",
            sol::no_constructor,

            "enable", [](gcomponent<scene>& c){
                c.owner().component<disabled<scene>>().remove();
            },

            "disable", [](gcomponent<scene>& c){
                c.owner().component<disabled<scene>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<scene>& c) -> bool {
                    return !c.owner().component<disabled<scene>>().exists();
                },
                [](gcomponent<scene>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<scene>>().remove();
                    } else {
                        c.owner().component<disabled<scene>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<scene>& c) -> bool {
                    return c.owner().component<disabled<scene>>().exists();
                },
                [](gcomponent<scene>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<scene>>().ensure();
                    } else {
                        c.owner().component<disabled<scene>>().remove();
                    }
                }
            ),

            "depth", sol::property(
                [](const gcomponent<scene>& c) -> i32 {
                    return c->depth();
                },
                [](gcomponent<scene>& c, i32 v){
                    c->depth(v);
                })
        );
    }
}
