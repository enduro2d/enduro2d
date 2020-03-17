/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class handle final {
    public:
        handle() = default;
    };
}

namespace e2d
{
    template <>
    class factory_loader<handle> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            handle& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<handle> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<handle>& c) const;
    };
}
