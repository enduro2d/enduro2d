/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../assets/flipbook_asset.hpp"

namespace e2d
{
    class flipbook_source final {
    public:
        flipbook_source() = default;
        flipbook_source(const flipbook_asset::ptr& flipbook);

        flipbook_source& flipbook(const flipbook_asset::ptr& value) noexcept;
        const flipbook_asset::ptr& flipbook() const noexcept;
    private:
        flipbook_asset::ptr flipbook_;
    };

    template <>
    class factory_loader<flipbook_source> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            flipbook_source& component,
            const fill_context& ctx) const;
            
        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    inline flipbook_source::flipbook_source(const flipbook_asset::ptr& flipbook)
    : flipbook_(flipbook) {}

    inline flipbook_source& flipbook_source::flipbook(const flipbook_asset::ptr& value) noexcept {
        flipbook_ = value;
        return *this;
    }

    inline const flipbook_asset::ptr& flipbook_source::flipbook() const noexcept {
        return flipbook_;
    }
}
