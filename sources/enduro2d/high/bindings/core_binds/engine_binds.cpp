/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_engine(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["core"].get_or_create<sol::table>()
        .new_usertype<engine>("engine",
            sol::no_constructor,

            "time", sol::property(&engine::time),
            "delta_time", sol::property(&engine::delta_time),
            "frame_rate", sol::property(&engine::frame_rate),
            "frame_count", sol::property(&engine::frame_count),
            "realtime_time", sol::property(&engine::realtime_time)
        );
    }
}
