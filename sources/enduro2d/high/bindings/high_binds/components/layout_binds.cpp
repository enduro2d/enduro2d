/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/layout.hpp>

namespace e2d::bindings::high
{
    void bind_layout(sol::state& l) {
        l.new_usertype<gcomponent<layout>>("layout",
            sol::no_constructor
        );

        l.new_usertype<gcomponent<layout_item>>("layout_item",
            sol::no_constructor
        );
    }
}
