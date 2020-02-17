/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/atlas_asset.hpp"
#include "../assets/sprite_asset.hpp"

namespace e2d
{
    class mask final {
    public:
        mask() = default;
        mask(const sprite_asset::ptr& sprite);

        mask& visible(bool value) noexcept;
        [[nodiscard]] bool visible() const noexcept;

        mask& sprite(const sprite_asset::ptr& value) noexcept;
        [[nodiscard]] const sprite_asset::ptr& sprite() const noexcept;

        mask& alpha_threshold(f32 value) noexcept;
        [[nodiscard]] f32 alpha_threshold() const noexcept;
    private:
        bool visible_ = false;
        sprite_asset::ptr sprite_;
        f32 alpha_threshold_ = 0.1f;
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

    inline mask& mask::sprite(const sprite_asset::ptr& value) noexcept {
        sprite_ = value;
        return *this;
    }

    inline const sprite_asset::ptr& mask::sprite() const noexcept {
        return sprite_;
    }

    inline mask& mask::alpha_threshold(f32 value) noexcept {
        alpha_threshold_ = math::clamp(value, 0.f, 1.f);
        return *this;
    }

    inline f32 mask::alpha_threshold() const noexcept {
        return alpha_threshold_;
    }
}
