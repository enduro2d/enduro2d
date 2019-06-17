/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/luasol.hpp>

namespace e2d::bindings
{
    void bind_math(sol::state& l);
    void bind_utils(sol::state& l);
    void bind_core(sol::state& l);
    void bind_high(sol::state& l);
}
