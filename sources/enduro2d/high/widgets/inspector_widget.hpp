/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../../core/dbgui_impl/dbgui.hpp"

namespace e2d::dbgui_widgets
{
    class inspector_widget final : public dbgui::widget {
    public:
        inspector_widget();
        ~inspector_widget() noexcept = default;

        bool show() override;
        const description& desc() const noexcept override;
    private:
        description desc_;
    };
}
