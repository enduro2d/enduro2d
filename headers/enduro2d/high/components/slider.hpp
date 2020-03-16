/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/button_style_asset.hpp"

namespace e2d
{
    class slider final {
    public:
        slider() = default;

        slider& style(const button_style_asset::ptr& value) noexcept;
        [[nodiscard]] const button_style_asset::ptr& style() const noexcept;
    private:
        button_style_asset::ptr style_;
    };
}

namespace e2d
{
    template <>
    class factory_loader<slider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            slider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<slider> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<slider>& c) const;
    };
}

namespace e2d
{
    inline slider& slider::style(const button_style_asset::ptr& value) noexcept {
        style_ = value;
        return *this;
    }

    inline const button_style_asset::ptr& slider::style() const noexcept {
        return style_;
    }
}
