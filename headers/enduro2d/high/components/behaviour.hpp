/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../assets/script_asset.hpp"

namespace e2d
{
    class behaviour final {
    public:
        class started final {};
        class disabled final {};
    public:
        behaviour() = default;
        behaviour(const script_asset::ptr& script);

        behaviour& meta(sol::table&& value) noexcept;
        behaviour& meta(const sol::table& value);

        [[nodiscard]] sol::table& meta() noexcept;
        [[nodiscard]] const sol::table& meta() const noexcept;

        behaviour& script(const script_asset::ptr& value) noexcept;
        [[nodiscard]] const script_asset::ptr& script() const noexcept;
    private:
        sol::table meta_;
        script_asset::ptr script_;
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

    template <>
    class factory_loader<behaviour::started> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            behaviour::started& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
    
    template <>
    class factory_loader<behaviour::disabled> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            behaviour::disabled& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    inline behaviour::behaviour(const script_asset::ptr& value)
    : script_(value) {}

    inline behaviour& behaviour::meta(sol::table&& value) noexcept {
        meta_ = std::move(value);
        return *this;
    }

    inline behaviour& behaviour::meta(const sol::table& value) {
        meta_ = value;
        return *this;
    }

    inline sol::table& behaviour::meta() noexcept {
        return meta_;
    }

    inline const sol::table& behaviour::meta() const noexcept {
        return meta_;
    }

    inline behaviour& behaviour::script(const script_asset::ptr& value) noexcept {
        script_ = value;
        return *this;
    }

    inline const script_asset::ptr& behaviour::script() const noexcept {
        return script_;
    }
}
