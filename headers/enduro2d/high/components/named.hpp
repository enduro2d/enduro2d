/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class named final {
    public:
        named() = default;

        named& name(str value) noexcept;

        [[nodiscard]] str& name() noexcept;
        [[nodiscard]] const str& name() const noexcept;
    private:
        str name_;
    };
}

namespace e2d
{
    template <>
    class factory_loader<named> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            named& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<named> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<named>& c) const;
    };
}

namespace e2d
{
    inline named& named::name(str value) noexcept {
        name_ = std::move(value);
        return *this;
    }

    inline str& named::name() noexcept {
        return name_;
    }

    inline const str& named::name() const noexcept {
        return name_;
    }
}
