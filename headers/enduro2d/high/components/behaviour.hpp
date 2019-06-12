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
    class behaviour final {
    public:
        behaviour() = default;
    };

    template <>
    class factory_loader<behaviour> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            behaviour& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}
