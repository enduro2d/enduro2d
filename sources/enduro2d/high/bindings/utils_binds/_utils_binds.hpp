/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#pragma once

#include "../bindings.hpp"

namespace e2d::bindings::utils
{
    void bind_color(sol::state& l);
    void bind_color32(sol::state& l);
    void bind_str_hash(sol::state& l);
}

namespace e2d::bindings
{
    inline void bind_utils(sol::state& l) {
        utils::bind_color(l);
        utils::bind_color32(l);
        utils::bind_str_hash(l);
    }
}
