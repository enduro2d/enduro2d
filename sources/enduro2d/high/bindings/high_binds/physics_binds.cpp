/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/physics.hpp>

namespace e2d::bindings::high
{
    void bind_physics(sol::state& l) {
        l.new_usertype<physics>("physics",
            sol::no_constructor
        );
    }
}
