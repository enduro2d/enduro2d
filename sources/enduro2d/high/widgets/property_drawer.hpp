/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/_high.hpp>

#include "../../core/dbgui_impl/dbgui.hpp"

namespace e2d::dbgui_widgets
{
    class property_drawer {
    public:
        property_drawer() = default;

        bool operator()(str_view l, f32& p) const;
        bool operator()(str_view l, property<f32>& p) const;
        bool operator()(str_view l, drag_property<f32>& p) const;
        bool operator()(str_view l, slider_property<f32>& p) const;

        bool operator()(str_view l, v2f& p) const;
        bool operator()(str_view l, property<v2f>& p) const;
        bool operator()(str_view l, drag_property<v2f>& p) const;
        bool operator()(str_view l, slider_property<v2f>& p) const;

        bool operator()(str_view l, v3f& p) const;
        bool operator()(str_view l, property<v3f>& p) const;
        bool operator()(str_view l, drag_property<v3f>& p) const;
        bool operator()(str_view l, slider_property<v3f>& p) const;

        bool operator()(str_view l, v4f& p) const;
        bool operator()(str_view l, property<v4f>& p) const;
        bool operator()(str_view l, drag_property<v4f>& p) const;
        bool operator()(str_view l, slider_property<v4f>& p) const;

        bool operator()(str_view l, str& p) const;
        bool operator()(str_view l, property<str>& p) const;
        bool operator()(str_view l, text_property& p) const;

        bool operator()(str_view l, degf& p) const;
        bool operator()(str_view l, property<degf>& p) const;
        bool operator()(str_view l, drag_property<degf>& p) const;
        bool operator()(str_view l, slider_property<degf>& p) const;

        bool operator()(str_view l, radf& p) const;
        bool operator()(str_view l, property<radf>& p) const;
        bool operator()(str_view l, drag_property<radf>& p) const;
        bool operator()(str_view l, slider_property<radf>& p) const;

        bool operator()(str_view l, color& p) const;
        bool operator()(str_view l, property<color>& p) const;

        bool operator()(str_view l, color32& p) const;
        bool operator()(str_view l, property<color32>& p) const;
    };
}
