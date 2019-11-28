/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/colliders.hpp>

namespace
{
    using namespace e2d;

    void bind_box_collider(sol::state& l) {
        l.new_usertype<gcomponent<box_collider>>("box_collider",
            sol::no_constructor
        );
    }

    void bind_circle_collider(sol::state& l) {
        l.new_usertype<gcomponent<circle_collider>>("circle_collider",
            sol::no_constructor
        );
    }

    void bind_polygon_collider(sol::state& l) {
        l.new_usertype<gcomponent<polygon_collider>>("polygon_collider",
            sol::no_constructor
        );
    }
}

namespace e2d::bindings::high
{
    void bind_colliders(sol::state& l) {
        bind_box_collider(l);
        bind_circle_collider(l);
        bind_polygon_collider(l);
    }
}
