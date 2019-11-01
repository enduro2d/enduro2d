/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_dbgui(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["core"].get_or_create<sol::table>()
        .new_usertype<dbgui>("dbgui",
            sol::no_constructor,

            "visible", sol::property(
                &dbgui::visible,
                &dbgui::toggle_visible)
        );
    }
}
