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
                c.component<disabled<slider>>().remove();
            },

            "disable", [](gcomponent<slider>& c){
                c.component<disabled<slider>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<slider>& c) -> bool {
                    return !c.component<disabled<slider>>().exists();
                },
                [](gcomponent<slider>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<slider>>().remove();
                    } else {
                        c.component<disabled<slider>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<slider>& c) -> bool {
                    return c.component<disabled<slider>>().exists();
                },
                [](gcomponent<slider>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<slider>>().ensure();
                    } else {
                        c.component<disabled<slider>>().remove();
                    }
                }
            ),

            "min_value", sol::property(
                [](const gcomponent<slider>& c) -> f32 {
                    return c->min_value();
                },
                [](gcomponent<slider>& c, f32 v){
                    c->min_value(v);
                }),

            "max_value", sol::property(
                [](const gcomponent<slider>& c) -> f32 {
                    return c->max_value();
                },
                [](gcomponent<slider>& c, f32 v){
                    c->max_value(v);
                }),

            "value", sol::property(
                [](const gcomponent<slider>& c) -> f32 {
                    return c->value();
                },
                [](gcomponent<slider>& c, f32 v){
                    c->value(v);
                }),

            "raw_value", sol::property(
                [](const gcomponent<slider>& c) -> f32 {
                    return c->raw_value();
                },
                [](gcomponent<slider>& c, f32 v){
                    c->raw_value(v);
                }),

            "normalized_value", sol::property(
                [](const gcomponent<slider>& c) -> f32 {
                    return c->normalized_value();
                },
                [](gcomponent<slider>& c, f32 v){
                    c->normalized_value(v);
                }),

            "normalized_raw_value", sol::property(
                [](const gcomponent<slider>& c) -> f32 {
                    return c->normalized_raw_value();
                },
                [](gcomponent<slider>& c, f32 v){
                    c->normalized_raw_value(v);
                }),

            "whole_numbers", sol::property(
                [](const gcomponent<slider>& c) -> bool {
                    return c->whole_numbers();
                },
                [](gcomponent<slider>& c, bool v){
                    c->whole_numbers(v);
                }),

            "direction", sol::property(
                [](const gcomponent<slider>& c) -> slider::directions {
                    return c->direction();
                },
                [](gcomponent<slider>& c, slider::directions v){
                    c->direction(v);
                })
        );
    }
}
