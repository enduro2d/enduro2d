/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/mask.hpp>

namespace e2d::bindings::high
{
    void bind_mask(sol::state& l) {
        l.new_usertype<gcomponent<mask>>("mask",
            sol::no_constructor,

            "enable", [](gcomponent<mask>& c){
                c.owner().component<disabled<mask>>().remove();
            },

            "disable", [](gcomponent<mask>& c){
                c.owner().component<disabled<mask>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<mask>& c) -> bool {
                    return !c.owner().component<disabled<mask>>().exists();
                },
                [](gcomponent<mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<mask>>().remove();
                    } else {
                        c.owner().component<disabled<mask>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<mask>& c) -> bool {
                    return c.owner().component<disabled<mask>>().exists();
                },
                [](gcomponent<mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<mask>>().ensure();
                    } else {
                        c.owner().component<disabled<mask>>().remove();
                    }
                }
            ),

            "visible", sol::property(
                [](const gcomponent<mask>& c) -> bool {
                    return c->visible();
                },
                [](gcomponent<mask>& c, bool v){
                    c->visible(v);
                }),

            "sprite", sol::property(
                [](const gcomponent<mask>& c) -> sprite_asset::ptr {
                    return c->sprite();
                },
                [](gcomponent<mask>& c, const sprite_asset::ptr& v){
                    c->sprite(v);
                }),

            "alpha_threshold", sol::property(
                [](const gcomponent<mask>& c) -> f32 {
                    return c->alpha_threshold();
                },
                [](gcomponent<mask>& c, f32 v){
                    c->alpha_threshold(v);
                })
        );
    }
}
