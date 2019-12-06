/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../../core/dbgui_impl/dbgui.hpp"

namespace e2d::dbgui_widgets
{
    class hierarchy_widget final : public dbgui::widget {
    public:
        hierarchy_widget();
        ~hierarchy_widget() noexcept = default;

        bool show() override;
        const description& desc() const noexcept override;
    private:
        description desc_;
    };
}
