/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class wlabel final {
    public:
        wlabel() = default;
    };
}

namespace e2d
{
    template <>
    class factory_loader<wlabel> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            wlabel& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<wlabel> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<wlabel>& c) const;
    };
}
