/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class mask final {
    public:
        mask() = default;

        mask& visible(bool value) noexcept;
        [[nodiscard]] bool visible() const noexcept;
    private:
        bool visible_ = false;
    };
}

namespace e2d
{
    template <>
    class factory_loader<mask> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            mask& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<mask> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<mask>& c) const;
    };
}

namespace e2d
{
    inline mask& mask::visible(bool value) noexcept {
        visible_ = value;
        return *this;
    }

    inline bool mask::visible() const noexcept {
        return visible_;
    }
}
