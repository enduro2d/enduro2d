/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../bindings.hpp"

namespace e2d::bindings::core
{
    void bind_dbgui(sol::state& l);
    void bind_debug(sol::state& l);
    void bind_engine(sol::state& l);
    void bind_input(sol::state& l);
    void bind_window(sol::state& l);
}

namespace e2d::bindings
{
    inline void bind_core(sol::state& l) {
        core::bind_dbgui(l);
        core::bind_debug(l);
        core::bind_engine(l);
        core::bind_input(l);
        core::bind_window(l);
    }
}
