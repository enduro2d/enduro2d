/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/atlas_asset.hpp"
#include "../assets/sprite_asset.hpp"

namespace e2d::impl
{
    template < typename Mask >
    class mask_base {
    public:
        mask_base() = default;

        Mask& offset(const v2f& value) noexcept;
        [[nodiscard]] const v2f& offset() const noexcept;

        Mask& visible(bool value) noexcept;
        [[nodiscard]] bool visible() const noexcept;
    private:
        v2f offset_ = v2f::zero();
        bool visible_ = false;
    };
}

namespace e2d
{
    class rect_mask final
        : public impl::mask_base<rect_mask> {
    public:
        rect_mask() = default;

        rect_mask& size(const v2f& value) noexcept;
        [[nodiscard]] const v2f& size() const noexcept;
    private:
        v2f size_ = v2f::unit();
    };

    class circle_mask final
        : public impl::mask_base<circle_mask> {
    public:
        circle_mask() = default;

        circle_mask& radius(f32 value) noexcept;
        [[nodiscard]] f32 radius() const noexcept;
    private:
        f32 radius_ = 1.f;
    };

    class sprite_mask final
        : public impl::mask_base<sprite_mask> {
    public:
        sprite_mask() = default;

        sprite_mask& sprite(const sprite_asset::ptr& value) noexcept;
        [[nodiscard]] const sprite_asset::ptr& sprite() const noexcept;

        sprite_mask& threshold(f32 value) noexcept;
        [[nodiscard]] f32 threshold() const noexcept;
    private:
        sprite_asset::ptr sprite_;
        f32 threshold_ = 0.1f;
    };
}

namespace e2d
{
    template <>
    class factory_loader<rect_mask> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            rect_mask& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<circle_mask> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            circle_mask& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<sprite_mask> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            sprite_mask& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<rect_mask> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<rect_mask>& c) const;
        void operator()(gcomponent<rect_mask>& c, gizmos_context& ctx) const;
    };

    template <>
    class component_inspector<circle_mask> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<circle_mask>& c) const;
        void operator()(gcomponent<circle_mask>& c, gizmos_context& ctx) const;
    };

    template <>
    class component_inspector<sprite_mask> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<sprite_mask>& c) const;
        void operator()(gcomponent<sprite_mask>& c, gizmos_context& ctx) const;
    };
}

namespace e2d::impl
{
    template < typename Mask >
    Mask& mask_base<Mask>::offset(const v2f& value) noexcept {
        offset_ = value;
        return static_cast<Mask&>(*this);
    }

    template < typename Mask >
    const v2f& mask_base<Mask>::offset() const noexcept {
        return offset_;
    }

    template < typename Mask >
    Mask& mask_base<Mask>::visible(bool value) noexcept {
        visible_ = value;
        return static_cast<Mask&>(*this);
    }

    template < typename Mask >
    bool mask_base<Mask>::visible() const noexcept {
        return visible_;
    }
}

namespace e2d
{
    inline rect_mask& rect_mask::size(const v2f& value) noexcept {
        size_ = math::maximized(v2f::unit(), value);
        return *this;
    }

    inline const v2f& rect_mask::size() const noexcept {
        return size_;
    }
}

namespace e2d
{
    inline circle_mask& circle_mask::radius(f32 value) noexcept {
        radius_ = math::max(1.f, value);
        return *this;
    }

    inline f32 circle_mask::radius() const noexcept {
        return radius_;
    }
}

namespace e2d
{
    inline sprite_mask& sprite_mask::sprite(const sprite_asset::ptr& value) noexcept {
        sprite_ = value;
        return *this;
    }

    inline const sprite_asset::ptr& sprite_mask::sprite() const noexcept {
        return sprite_;
    }

    inline sprite_mask& sprite_mask::threshold(f32 value) noexcept {
        threshold_ = math::clamp(value, 0.f, 1.f);
        return *this;
    }

    inline f32 sprite_mask::threshold() const noexcept {
        return threshold_;
    }
}
