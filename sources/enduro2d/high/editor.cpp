/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/editor.hpp>

#include <enduro2d/high/widgets/hierarchy_widget.hpp>
#include <enduro2d/high/widgets/inspector_widget.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    editor::editor() {
        if ( modules::is_initialized<dbgui>() ) {
            the<dbgui>().register_menu_widget<dbgui_widgets::hierarchy_widget>("Scene", "Hierarchy");
            the<dbgui>().register_menu_widget<dbgui_widgets::inspector_widget>("Scene", "Inspector");
        }
    }

    editor::~editor() noexcept = default;
}
