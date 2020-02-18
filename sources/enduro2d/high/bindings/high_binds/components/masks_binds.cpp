/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/masks.hpp>

namespace
{
    using namespace e2d;

    void bind_rect_mask(sol::state& l) {
        l.new_usertype<gcomponent<rect_mask>>("rect_mask",
            sol::no_constructor,

            "enable", [](gcomponent<rect_mask>& c){
                c.owner().component<disabled<rect_mask>>().remove();
            },

            "disable", [](gcomponent<rect_mask>& c){
                c.owner().component<disabled<rect_mask>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<rect_mask>& c) -> bool {
                    return !c.owner().component<disabled<rect_mask>>().exists();
                },
                [](gcomponent<rect_mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<rect_mask>>().remove();
                    } else {
                        c.owner().component<disabled<rect_mask>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<rect_mask>& c) -> bool {
                    return c.owner().component<disabled<rect_mask>>().exists();
                },
                [](gcomponent<rect_mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<rect_mask>>().ensure();
                    } else {
                        c.owner().component<disabled<rect_mask>>().remove();
                    }
                }
            ),

            "size", sol::property(
                [](const gcomponent<rect_mask>& c) -> v2f {
                    return c->size();
                },
                [](gcomponent<rect_mask>& c, const v2f& v){
                    c->size(v);
                }),

            "offset", sol::property(
                [](const gcomponent<rect_mask>& c) -> v2f {
                    return c->offset();
                },
                [](gcomponent<rect_mask>& c, const v2f& v){
                    c->offset(v);
                }),

            "visible", sol::property(
                [](const gcomponent<rect_mask>& c) -> bool {
                    return c->visible();
                },
                [](gcomponent<rect_mask>& c, bool v){
                    c->visible(v);
                })
        );
    }

    void bind_circle_mask(sol::state& l) {
        l.new_usertype<gcomponent<circle_mask>>("circle_mask",
            sol::no_constructor,

            "enable", [](gcomponent<circle_mask>& c){
                c.owner().component<disabled<circle_mask>>().remove();
            },

            "disable", [](gcomponent<circle_mask>& c){
                c.owner().component<disabled<circle_mask>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<circle_mask>& c) -> bool {
                    return !c.owner().component<disabled<circle_mask>>().exists();
                },
                [](gcomponent<circle_mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<circle_mask>>().remove();
                    } else {
                        c.owner().component<disabled<circle_mask>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<circle_mask>& c) -> bool {
                    return c.owner().component<disabled<circle_mask>>().exists();
                },
                [](gcomponent<circle_mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<circle_mask>>().ensure();
                    } else {
                        c.owner().component<disabled<circle_mask>>().remove();
                    }
                }
            ),

            "radius", sol::property(
                [](const gcomponent<circle_mask>& c) -> f32 {
                    return c->radius();
                },
                [](gcomponent<circle_mask>& c, f32 v){
                    c->radius(v);
                }),

            "offset", sol::property(
                [](const gcomponent<circle_mask>& c) -> v2f {
                    return c->offset();
                },
                [](gcomponent<circle_mask>& c, const v2f& v){
                    c->offset(v);
                }),

            "visible", sol::property(
                [](const gcomponent<circle_mask>& c) -> bool {
                    return c->visible();
                },
                [](gcomponent<circle_mask>& c, bool v){
                    c->visible(v);
                })
        );
    }

    void bind_sprite_mask(sol::state& l) {
        l.new_usertype<gcomponent<sprite_mask>>("sprite_mask",
            sol::no_constructor,

            "enable", [](gcomponent<sprite_mask>& c){
                c.owner().component<disabled<sprite_mask>>().remove();
            },

            "disable", [](gcomponent<sprite_mask>& c){
                c.owner().component<disabled<sprite_mask>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<sprite_mask>& c) -> bool {
                    return !c.owner().component<disabled<sprite_mask>>().exists();
                },
                [](gcomponent<sprite_mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<sprite_mask>>().remove();
                    } else {
                        c.owner().component<disabled<sprite_mask>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<sprite_mask>& c) -> bool {
                    return c.owner().component<disabled<sprite_mask>>().exists();
                },
                [](gcomponent<sprite_mask>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<disabled<sprite_mask>>().ensure();
                    } else {
                        c.owner().component<disabled<sprite_mask>>().remove();
                    }
                }
            ),

            "sprite", sol::property(
                [](const gcomponent<sprite_mask>& c) -> sprite_asset::ptr {
                    return c->sprite();
                },
                [](gcomponent<sprite_mask>& c, const sprite_asset::ptr& v){
                    c->sprite(v);
                }),

            "threshold", sol::property(
                [](const gcomponent<sprite_mask>& c) -> f32 {
                    return c->threshold();
                },
                [](gcomponent<sprite_mask>& c, f32 v){
                    c->threshold(v);
                }),

            "offset", sol::property(
                [](const gcomponent<sprite_mask>& c) -> v2f {
                    return c->offset();
                },
                [](gcomponent<sprite_mask>& c, const v2f& v){
                    c->offset(v);
                }),

            "visible", sol::property(
                [](const gcomponent<sprite_mask>& c) -> bool {
                    return c->visible();
                },
                [](gcomponent<sprite_mask>& c, bool v){
                    c->visible(v);
                })
        );
    }
}

namespace e2d::bindings::high
{
    void bind_masks(sol::state& l) {
        bind_rect_mask(l);
        bind_circle_mask(l);
        bind_sprite_mask(l);
    }
}
