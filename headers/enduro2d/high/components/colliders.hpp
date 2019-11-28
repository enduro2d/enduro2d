/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"

namespace e2d
{
    class box_collider final {
    public:
        box_collider() = default;
    };

    class circle_collider final {
    public:
        circle_collider() = default;
    };

    class polygon_collider final {
    public:
        polygon_collider() = default;
    };
}

namespace e2d
{
    template <>
    class factory_loader<box_collider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            box_collider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<circle_collider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            circle_collider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<polygon_collider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            polygon_collider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}
