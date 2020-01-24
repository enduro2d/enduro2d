/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/colliders.hpp>

namespace
{
    using namespace e2d;

    void bind_rect_collider(sol::state& l) {
        l.new_usertype<gcomponent<rect_collider>>("rect_collider",
            sol::no_constructor,

            "enable", [](gcomponent<rect_collider>& c){
                c.owner().component<disabled<rect_collider>>().remove();
            },

            "disable", [](gcomponent<rect_collider>& c){
                c.owner().component<disabled<rect_collider>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<rect_collider>& c) -> bool {
                    return !c.owner().component<disabled<rect_collider>>().exists();
                },
                [](gcomponent<rect_collider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<rect_collider>>().remove();
                    } else {
                        c.owner().component<disabled<rect_collider>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<rect_collider>& c) -> bool {
                    return c.owner().component<disabled<rect_collider>>().exists();
                },
                [](gcomponent<rect_collider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<rect_collider>>().ensure();
                    } else {
                        c.owner().component<disabled<rect_collider>>().remove();
                    }
                }
            ),

            "size", sol::property(
                [](const gcomponent<rect_collider>& c) -> v2f {
                    return c->size();
                },
                [](gcomponent<rect_collider>& c, const v2f& v){
                    c->size(v);
                }),

            "offset", sol::property(
                [](const gcomponent<rect_collider>& c) -> v2f {
                    return c->offset();
                },
                [](gcomponent<rect_collider>& c, const v2f& v){
                    c->offset(v);
                }),

            "sensor", sol::property(
                [](const gcomponent<rect_collider>& c) -> bool {
                    return c->sensor();
                },
                [](gcomponent<rect_collider>& c, bool v){
                    c->sensor(v);
                }),

            "density", sol::property(
                [](const gcomponent<rect_collider>& c) -> f32 {
                    return c->density();
                },
                [](gcomponent<rect_collider>& c, f32 v){
                    c->density(v);
                }),

            "friction", sol::property(
                [](const gcomponent<rect_collider>& c) -> f32 {
                    return c->friction();
                },
                [](gcomponent<rect_collider>& c, f32 v){
                    c->friction(v);
                }),

            "restitution", sol::property(
                [](const gcomponent<rect_collider>& c) -> f32 {
                    return c->restitution();
                },
                [](gcomponent<rect_collider>& c, f32 v){
                    c->restitution(v);
                })
        );
    }

    void bind_circle_collider(sol::state& l) {
        l.new_usertype<gcomponent<circle_collider>>("circle_collider",
            sol::no_constructor,

            "enable", [](gcomponent<circle_collider>& c){
                c.owner().component<disabled<circle_collider>>().remove();
            },

            "disable", [](gcomponent<circle_collider>& c){
                c.owner().component<disabled<circle_collider>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<circle_collider>& c) -> bool {
                    return !c.owner().component<disabled<circle_collider>>().exists();
                },
                [](gcomponent<circle_collider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<circle_collider>>().remove();
                    } else {
                        c.owner().component<disabled<circle_collider>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<circle_collider>& c) -> bool {
                    return c.owner().component<disabled<circle_collider>>().exists();
                },
                [](gcomponent<circle_collider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<circle_collider>>().ensure();
                    } else {
                        c.owner().component<disabled<circle_collider>>().remove();
                    }
                }
            ),

            "radius", sol::property(
                [](const gcomponent<circle_collider>& c) -> f32 {
                    return c->radius();
                },
                [](gcomponent<circle_collider>& c, f32 v){
                    c->radius(v);
                }),

            "offset", sol::property(
                [](const gcomponent<circle_collider>& c) -> v2f {
                    return c->offset();
                },
                [](gcomponent<circle_collider>& c, const v2f& v){
                    c->offset(v);
                }),

            "sensor", sol::property(
                [](const gcomponent<circle_collider>& c) -> bool {
                    return c->sensor();
                },
                [](gcomponent<circle_collider>& c, bool v){
                    c->sensor(v);
                }),

            "density", sol::property(
                [](const gcomponent<circle_collider>& c) -> f32 {
                    return c->density();
                },
                [](gcomponent<circle_collider>& c, f32 v){
                    c->density(v);
                }),

            "friction", sol::property(
                [](const gcomponent<circle_collider>& c) -> f32 {
                    return c->friction();
                },
                [](gcomponent<circle_collider>& c, f32 v){
                    c->friction(v);
                }),

            "restitution", sol::property(
                [](const gcomponent<circle_collider>& c) -> f32 {
                    return c->restitution();
                },
                [](gcomponent<circle_collider>& c, f32 v){
                    c->restitution(v);
                })
        );
    }

    void bind_polygon_collider(sol::state& l) {
        l.new_usertype<gcomponent<polygon_collider>>("polygon_collider",
            sol::no_constructor,

            "enable", [](gcomponent<polygon_collider>& c){
                c.owner().component<disabled<polygon_collider>>().remove();
            },

            "disable", [](gcomponent<polygon_collider>& c){
                c.owner().component<disabled<polygon_collider>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<polygon_collider>& c) -> bool {
                    return !c.owner().component<disabled<polygon_collider>>().exists();
                },
                [](gcomponent<polygon_collider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<polygon_collider>>().remove();
                    } else {
                        c.owner().component<disabled<polygon_collider>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<polygon_collider>& c) -> bool {
                    return c.owner().component<disabled<polygon_collider>>().exists();
                },
                [](gcomponent<polygon_collider>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<polygon_collider>>().ensure();
                    } else {
                        c.owner().component<disabled<polygon_collider>>().remove();
                    }
                }
            ),

            "points", sol::property(
                [](const gcomponent<polygon_collider>& c) -> vector<v2f> {
                    return c->points();
                },
                [](gcomponent<polygon_collider>& c, vector<v2f> v){
                    c->points(std::move(v));
                }),

            "offset", sol::property(
                [](const gcomponent<polygon_collider>& c) -> v2f {
                    return c->offset();
                },
                [](gcomponent<polygon_collider>& c, const v2f& v){
                    c->offset(v);
                }),

            "sensor", sol::property(
                [](const gcomponent<polygon_collider>& c) -> bool {
                    return c->sensor();
                },
                [](gcomponent<polygon_collider>& c, bool v){
                    c->sensor(v);
                }),

            "density", sol::property(
                [](const gcomponent<polygon_collider>& c) -> f32 {
                    return c->density();
                },
                [](gcomponent<polygon_collider>& c, f32 v){
                    c->density(v);
                }),

            "friction", sol::property(
                [](const gcomponent<polygon_collider>& c) -> f32 {
                    return c->friction();
                },
                [](gcomponent<polygon_collider>& c, f32 v){
                    c->friction(v);
                }),

            "restitution", sol::property(
                [](const gcomponent<polygon_collider>& c) -> f32 {
                    return c->restitution();
                },
                [](gcomponent<polygon_collider>& c, f32 v){
                    c->restitution(v);
                })
        );
    }
}

namespace e2d::bindings::high
{
    void bind_colliders(sol::state& l) {
        bind_rect_collider(l);
        bind_circle_collider(l);
        bind_polygon_collider(l);
    }
}
