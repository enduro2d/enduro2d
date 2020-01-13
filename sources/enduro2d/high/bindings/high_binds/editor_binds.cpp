/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/editor.hpp>

namespace e2d::bindings::high
{
    void bind_editor(sol::state& l) {
        l.new_usertype<editor>("editor",
            sol::no_constructor,

            "select", [](editor& e, const gobject& go) -> void {
                e.select(go);
            },

            "clear_selection", [](editor& e) -> void {
                e.clear_selection();
            },

            "selection", sol::property(
                [](const editor& e) -> gobject { return e.selection(); })
        );
    }
}
