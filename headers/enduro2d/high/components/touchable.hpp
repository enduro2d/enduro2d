/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class touchable final {
    public:
        class touched final {};
        class under_mouse final {};
    public:
        touchable() = default;

        touchable& bubbling(bool value) noexcept;
        touchable& capturing(bool value) noexcept;

        [[nodiscard]] bool bubbling() const noexcept;
        [[nodiscard]] bool capturing() const noexcept;
    private:
        enum flag_masks : u32 {
            fm_bubbling = 1u << 0,
            fm_capturing = 1u << 1,
        };
    private:
        u32 flags_{
            fm_bubbling |
            fm_capturing};
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

    template <>
    class factory_loader<touchable::touched> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            touchable::touched& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<touchable::under_mouse> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            touchable::under_mouse& component,
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

namespace e2d
{
    inline touchable& touchable::bubbling(bool value) noexcept {
        if ( value != bubbling() ) {
            math::flip_flags_inplace(flags_, fm_bubbling);
        }
        return *this;
    }

    inline touchable& touchable::capturing(bool value) noexcept {
        if ( value != capturing() ) {
            math::flip_flags_inplace(flags_, fm_capturing);
        }
        return *this;
    }

    inline bool touchable::bubbling() const noexcept {
        return math::check_any_flags(flags_, fm_bubbling);
    }

    inline bool touchable::capturing() const noexcept {
        return math::check_any_flags(flags_, fm_capturing);
    }
}
