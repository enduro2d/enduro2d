/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class toggle_group final {
    public:
        toggle_group() = default;

        toggle_group& allow_switch_off(bool value) noexcept;
        [[nodiscard]] bool allow_switch_off() const noexcept;

        toggle_group& allow_multiple_on(bool value) noexcept;
        [[nodiscard]] bool allow_multiple_on() const noexcept;
    private:
        bool allow_switch_off_ = false;
        bool allow_multiple_on_ = false;
    };
}

namespace e2d
{
    template <>
    class factory_loader<toggle_group> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            toggle_group& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<toggle_group> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<toggle_group>& c) const;
    };
}

namespace e2d
{
    inline toggle_group& toggle_group::allow_switch_off(bool value) noexcept {
        allow_switch_off_ = value;
        return *this;
    }

    inline bool toggle_group::allow_switch_off() const noexcept {
        return allow_switch_off_;
    }

    inline toggle_group& toggle_group::allow_multiple_on(bool value) noexcept {
        allow_multiple_on_ = value;
        return *this;
    }

    inline bool toggle_group::allow_multiple_on() const noexcept {
        return allow_multiple_on_;
    }
}
