/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/camera.hpp>

namespace e2d::bindings::high
{
    void bind_camera(sol::state& l) {
        l.new_usertype<gcomponent<camera>>("camera",
            sol::no_constructor,

            "enable", [](gcomponent<camera>& c){
                c.owner().component<disabled<camera>>().remove();
            },

            "disable", [](gcomponent<camera>& c){
                c.owner().component<disabled<camera>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<camera>& c) -> bool {
                    return !c.owner().component<disabled<camera>>().exists();
                },
                [](gcomponent<camera>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<camera>>().remove();
                    } else {
                        c.owner().component<disabled<camera>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<camera>& c) -> bool {
                    return c.owner().component<disabled<camera>>().exists();
                },
                [](gcomponent<camera>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<camera>>().ensure();
                    } else {
                        c.owner().component<disabled<camera>>().remove();
                    }
                }
            ),

            "depth", sol::property(
                [](const gcomponent<camera>& c) -> i32 {
                    return c->depth();
                },
                [](gcomponent<camera>& c, i32 v){
                    c->depth(v);
                }),

            "viewport", sol::property(
                [](const gcomponent<camera>& c) -> b2f {
                    return c->viewport().cast_to<f32>();
                },
                [](gcomponent<camera>& c, const b2f& v){
                    c->viewport(v.cast_to<u32>());
                }),

            "projection", sol::property(
                [](const gcomponent<camera>& c) -> m4f {
                    return c->projection();
                },
                [](gcomponent<camera>& c, const m4f& v){
                    c->projection(v);
                }),

            "background", sol::property(
                [](const gcomponent<camera>& c) -> color {
                    return c->background();
                },
                [](gcomponent<camera>& c, const color& v){
                    c->background(v);
                })
        );
    }
}
