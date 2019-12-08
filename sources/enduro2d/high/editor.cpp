/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/editor.hpp>

#include <enduro2d/high/widgets/hierarchy_widget.hpp>
#include <enduro2d/high/widgets/inspector_widget.hpp>

namespace e2d
{
    editor::editor() {
        if ( modules::is_initialized<dbgui>() ) {
            the<dbgui>().register_menu_widget<dbgui_widgets::hierarchy_widget>("Scene", "Hierarchy");
            the<dbgui>().register_menu_widget<dbgui_widgets::inspector_widget>("Scene", "Inspector");
        }
    }

    editor::~editor() noexcept = default;

    void editor::select(const gobject& go) {
        std::lock_guard<std::mutex> guard(mutex_);
        selection_.insert(go);
    }

    bool editor::is_selected(const gobject& go) const noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        return selection_.find(go) != selection_.end();
    }

    void editor::unselect(const gobject& go) noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        selection_.erase(go);
    }

    void editor::clear_selection() noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        selection_.clear();
    }
}
