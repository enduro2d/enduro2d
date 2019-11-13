/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/named.hpp>

namespace e2d::bindings::high
{
    void bind_named(sol::state& l) {
        l.new_usertype<gcomponent<named>>("named",
            sol::no_constructor,

            "name", sol::property(
                [](const gcomponent<named>& c) -> str {
                    return c->name();
                },
                [](gcomponent<named>& c, str_view v){
                    c->name(str(v));
                })
        );
    }
}
