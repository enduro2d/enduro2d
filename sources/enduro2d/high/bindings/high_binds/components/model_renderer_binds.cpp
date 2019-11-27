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
        l.new_usertype<gcomponent<model_renderer>>("model_renderer",
            sol::no_constructor,

            "model", sol::property(
                [](const gcomponent<model_renderer>& c) -> model_asset::ptr {
                    return c->model();
                },
                [](gcomponent<model_renderer>& c, const model_asset::ptr& v){
                    c->model(v);
                })
        );
    }
}
