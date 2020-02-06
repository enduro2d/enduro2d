/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/actor.hpp>

namespace e2d::bindings::high
{
    void bind_actor(sol::state& l) {
        l.new_usertype<gcomponent<actor>>("actor",
            sol::no_constructor,

            "node", sol::property([](gcomponent<actor>& c) -> node_iptr {
                return c->node();
            })
        );
    }
}
