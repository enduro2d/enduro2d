/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/model_renderer.hpp>

namespace e2d::bindings::high
{
    void bind_model_renderer(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<model_renderer>>("model_renderer",
            "model", sol::property(
                [](const gcomponent<model_renderer>& l){ return l->model(); },
                [](gcomponent<model_renderer>& l, const model_asset::ptr& v){ l->model(v); })
        );
    }
}
