/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class progress final {
    public:
        ENUM_HPP_CLASS_DECL(directions, u8,
            (row)
            (row_reversed)
            (column)
            (column_reversed))
    public:
        progress() = default;

        progress& value(f32 value) noexcept;
        [[nodiscard]] f32 value() const noexcept;

        progress& direction(directions value) noexcept;
        [[nodiscard]] directions direction() const noexcept;
    private:
        f32 value_ = 0.f;
        directions direction_ = directions::row;
    };

    ENUM_HPP_REGISTER_TRAITS(progress::directions)
}

namespace e2d
{
    template <>
    class factory_loader<progress> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            progress& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<progress> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<progress>& c) const;
    };
}

namespace e2d
{
    inline progress& progress::value(f32 value) noexcept {
        value_ = math::saturate(value);
        return *this;
    }

    inline f32 progress::value() const noexcept {
        return value_;
    }

    inline progress& progress::direction(directions value) noexcept {
        direction_ = value;
        return *this;
    }

    inline progress::directions progress::direction() const noexcept {
        return direction_;
    }
}
