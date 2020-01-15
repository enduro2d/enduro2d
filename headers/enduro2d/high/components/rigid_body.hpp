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
    class rigid_body final {
    public:
        ENUM_HPP_CLASS_DECL(types, u8,
            (dynamic)
            (kinematic))
    public:
        rigid_body() = default;

        rigid_body& type(types value) noexcept;
        [[nodiscard]] types type() const noexcept;
    private:
        types type_ = types::dynamic;
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::rigid_body::types)

namespace e2d
{
    template <>
    class factory_loader<rigid_body> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            rigid_body& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<rigid_body> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<rigid_body>& c) const;
    };
}

namespace e2d
{
    inline rigid_body& rigid_body::type(types value) noexcept {
        type_ = value;
        return *this;
    }

    inline rigid_body::types rigid_body::type() const noexcept {
        return type_;
    }
}
