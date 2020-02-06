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

            "input", sol::property(
                [](const gcomponent<camera>& c) -> bool {
                    return c.owner().component<camera::input>().exists();
                },
                [](gcomponent<camera>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<camera::input>().ensure();
                    } else {
                        c.owner().component<camera::input>().remove();
                    }
                }
            ),

            "gizmos", sol::property(
                [](const gcomponent<camera>& c) -> bool {
                    return c.owner().component<camera::gizmos>().exists();
                },
                [](gcomponent<camera>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<camera::gizmos>().ensure();
                    } else {
                        c.owner().component<camera::gizmos>().remove();
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

            "mode", sol::property(
                [](const gcomponent<camera>& c) -> camera::modes {
                    return c->mode();
                },
                [](gcomponent<camera>& c, camera::modes v){
                    c->mode(v);
                }),

            "znear", sol::property(
                [](const gcomponent<camera>& c) -> f32 {
                    return c->znear();
                },
                [](gcomponent<camera>& c, f32 v){
                    c->znear(v);
                }),

            "zfar", sol::property(
                [](const gcomponent<camera>& c) -> f32 {
                    return c->zfar();
                },
                [](gcomponent<camera>& c, f32 v){
                    c->zfar(v);
                }),

            "view", sol::property(
                [](const gcomponent<camera>& c) -> m4f {
                    return c->view();
                },
                [](gcomponent<camera>& c, const m4f& v){
                    c->view(v);
                }),

            "viewport", sol::property(
                [](const gcomponent<camera>& c) -> b2f {
                    return c->viewport();
                },
                [](gcomponent<camera>& c, const b2f& v){
                    c->viewport(v);
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

    #define CAMERA_MODE_PAIR(x) {#x, camera::modes::x},
        l["camera"].get_or_create<sol::table>()
        .new_enum<camera::modes>("modes", {
            CAMERA_MODE_PAIR(manual)
            CAMERA_MODE_PAIR(stretch)
            CAMERA_MODE_PAIR(flexible)
            CAMERA_MODE_PAIR(fixed_fit)
            CAMERA_MODE_PAIR(fixed_crop)
        });
    #undef CAMERA_MODE_PAIR
    }
}
