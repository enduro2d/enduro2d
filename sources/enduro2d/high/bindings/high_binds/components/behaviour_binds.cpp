/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/behaviour.hpp>

namespace e2d::bindings::high
{
    void bind_behaviour(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<behaviour>>("behaviour",
            "meta", sol::property(
                [](const gcomponent<behaviour>& b){ return b->meta(); },
                [](gcomponent<behaviour>& b, sol::table v){ b->meta(std::move(v)); })
        );
    }
}
