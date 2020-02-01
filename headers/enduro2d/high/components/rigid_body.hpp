/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class rigid_body final {
    public:
        ENUM_HPP_CLASS_DECL(types, u8,
            (dynamic)
            (kinematic))

        ENUM_HPP_CLASS_DECL(sleepings, u8,
            (never)
            (start_awake)
            (start_asleep))

        ENUM_HPP_CLASS_DECL(collisions, u8,
            (discrete)
            (continuous))
    public:
        rigid_body() = default;

        rigid_body& type(types value) noexcept;
        [[nodiscard]] types type() const noexcept;

        rigid_body& sleeping(sleepings value) noexcept;
        [[nodiscard]] sleepings sleeping() const noexcept;

        rigid_body& collision(collisions value) noexcept;
        [[nodiscard]] collisions collision() const noexcept;

        rigid_body& gravity_scale(f32 value) noexcept;
        [[nodiscard]] f32 gravity_scale() const noexcept;

        rigid_body& fixed_rotation(bool value) noexcept;
        [[nodiscard]] bool fixed_rotation() const noexcept;
    private:
        types type_ = types::dynamic;
        sleepings sleeping_ = sleepings::start_awake;
        collisions collision_ = collisions::discrete;
        f32 gravity_scale_ = 1.f;
        bool fixed_rotation_ = false;
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::rigid_body::types)
ENUM_HPP_REGISTER_TRAITS(e2d::rigid_body::sleepings)
ENUM_HPP_REGISTER_TRAITS(e2d::rigid_body::collisions)

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

    inline rigid_body& rigid_body::sleeping(sleepings value) noexcept {
        sleeping_ = value;
        return *this;
    }

    inline rigid_body::sleepings rigid_body::sleeping() const noexcept {
        return sleeping_;
    }

    inline rigid_body& rigid_body::collision(collisions value) noexcept {
        collision_ = value;
        return *this;
    }

    inline rigid_body::collisions rigid_body::collision() const noexcept {
        return collision_;
    }

    inline rigid_body& rigid_body::gravity_scale(f32 value) noexcept {
        gravity_scale_ = value;
        return *this;
    }

    inline f32 rigid_body::gravity_scale() const noexcept {
        return gravity_scale_;
    }

    inline rigid_body& rigid_body::fixed_rotation(bool value) noexcept {
        fixed_rotation_ = value;
        return *this;
    }

    inline bool rigid_body::fixed_rotation() const noexcept {
        return fixed_rotation_;
    }
}
