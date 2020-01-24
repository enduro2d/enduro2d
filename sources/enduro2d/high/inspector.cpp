/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/inspector.hpp>

namespace e2d
{
    void inspector::show_for(gobject& go) {
        std::lock_guard<std::mutex> guard(mutex_);
        for ( auto& p : drawers_ ) {
            (*p.second)(go);
        }
    }

    void inspector::show_for(gobject& go, component_inspector<>::gizmos_context& ctx) {
        std::lock_guard<std::mutex> guard(mutex_);
        for ( auto& p : drawers_ ) {
            (*p.second)(go, ctx);
        }
    }
}
