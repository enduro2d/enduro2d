/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../dbgui.hpp"

namespace e2d::dbgui_widgets
{
    class window_widget final : public dbgui::widget {
    public:
        window_widget() = default;
        ~window_widget() noexcept = default;

        bool show() override;
        const description& desc() const noexcept override;
    private:
        description desc_;
    };
}
