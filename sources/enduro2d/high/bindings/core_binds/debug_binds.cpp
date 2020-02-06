/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_debug(sol::state& l) {
        l.new_usertype<debug>("debug",
            sol::no_constructor,

            "trace", [](debug& d, str_view s){
                d.trace(s);
            },

            "warning", [](debug& d, str_view s){
                d.warning(s);
            },

            "error", [](debug& d, str_view s){
                d.error(s);
            },

            "fatal", [](debug& d, str_view s){
                d.fatal(s);
            }
        );
    }
}
