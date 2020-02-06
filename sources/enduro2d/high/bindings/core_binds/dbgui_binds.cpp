/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace e2d::bindings::core
{
    void bind_dbgui(sol::state& l) {
        l.new_usertype<dbgui>("dbgui",
            sol::no_constructor,

            "visible", sol::property(
                [](const dbgui& d) -> bool {
                    return d.visible();
                },
                [](dbgui& d, bool yesno){
                    d.toggle_visible(yesno);
                })
        );
    }
}
