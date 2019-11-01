/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_debug(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["core"].get_or_create<sol::table>()
        .new_usertype<debug>("debug",
            sol::no_constructor,

            "trace", [](debug& d, const char* s){
                d.trace(s);
            },

            "warning", [](debug& d, const char* s){
                d.warning(s);
            },

            "error", [](debug& d, const char* s){
                d.error(s);
            },

            "fatal", [](debug& d, const char* s){
                d.fatal(s);
            }
        );
    }
}
