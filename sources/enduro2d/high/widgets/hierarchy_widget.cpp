/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "hierarchy_widget.hpp"

namespace e2d::dbgui_widgets
{
    hierarchy_widget::hierarchy_widget() {
        desc_.title = "Hierarchy";
        desc_.first_size = v2f(300.f, 400.f);
    }

    bool hierarchy_widget::show() {
        return true;
    }

    const hierarchy_widget::description& hierarchy_widget::desc() const noexcept {
        return desc_;
    }
}
