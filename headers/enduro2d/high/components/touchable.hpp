/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../gobject.hpp"
#include "../inspector.hpp"

namespace e2d
{
    class touchable final {
    public:
        touchable() = default;
    };
}

namespace e2d
{
    template <>
    class factory_loader<touchable> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            touchable& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<touchable> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<touchable>& c) const;
    };
}
