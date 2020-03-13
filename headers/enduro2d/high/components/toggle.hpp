/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/toggle_style_asset.hpp"

namespace e2d
{
    class toggle final {
    public:
        class pressed final {};
    public:
        toggle() = default;

        toggle& style(const toggle_style_asset::ptr& value) noexcept;
        [[nodiscard]] const toggle_style_asset::ptr& style() const noexcept;
    private:
        toggle_style_asset::ptr style_;
    };
}

namespace e2d
{
    template <>
    class factory_loader<toggle> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            toggle& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<toggle::pressed> final
    : public empty_factory_loader<toggle::pressed> {};
}

namespace e2d
{
    template <>
    class component_inspector<toggle> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<toggle>& c) const;
    };
}

namespace e2d
{
    inline toggle& toggle::style(const toggle_style_asset::ptr& value) noexcept {
        style_ = value;
        return *this;
    }

    inline const toggle_style_asset::ptr& toggle::style() const noexcept {
        return style_;
    }
}
