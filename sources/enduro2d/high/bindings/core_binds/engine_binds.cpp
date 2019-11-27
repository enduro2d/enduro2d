/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_engine(sol::state& l) {
        l.new_usertype<engine>("engine",
            sol::no_constructor,

            "time", sol::property([](const engine& e) -> f32 {
                return e.time();
            }),

            "delta_time", sol::property([](const engine& e) -> f32 {
                return e.delta_time();
            }),

            "frame_rate", sol::property([](const engine& e) -> u32 {
                return e.frame_rate();
            }),

            "frame_count", sol::property([](const engine& e) -> u32 {
                return e.frame_count();
            }),

            "realtime_time", sol::property([](const engine& e) -> f32 {
                return e.realtime_time();
            })
        );
    }
}
