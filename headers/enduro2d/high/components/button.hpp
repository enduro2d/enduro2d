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
    class button final {
    public:
        class pressed final {};
    public:
        button() = default;

        button& style(const button_style_asset::ptr& value) noexcept;
        [[nodiscard]] const button_style_asset::ptr& style() const noexcept;
    private:
        button_style_asset::ptr style_;
    };
}

namespace e2d
{
    template <>
    class factory_loader<button> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            button& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<button::pressed> final
    : public empty_factory_loader<button::pressed> {};
}

namespace e2d
{
    template <>
    class component_inspector<button> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<button>& c) const;
    };
}

namespace e2d
{
    inline button& button::style(const button_style_asset::ptr& value) noexcept {
        style_ = value;
        return *this;
    }

    inline const button_style_asset::ptr& button::style() const noexcept {
        return style_;
    }
}
