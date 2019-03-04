/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/sprite_asset.hpp"

namespace e2d
{
    class sprite_renderer final {
    public:
        sprite_renderer() = default;
        sprite_renderer(const sprite_asset::ptr& sprite);

        sprite_renderer& tint(const color& value) noexcept;
        const color& tint() const noexcept;

        sprite_renderer& sprite(const sprite_asset::ptr& value) noexcept;
        const sprite_asset::ptr& sprite() const noexcept;

        render::property_block& properties() noexcept;
        const render::property_block& properties() const noexcept;
    private:
        color tint_;
        sprite_asset::ptr sprite_;
        render::property_block properties_;
    };
}

namespace e2d
{
    inline sprite_renderer::sprite_renderer(const sprite_asset::ptr& sprite)
    : sprite_(sprite) {}

    inline sprite_renderer& sprite_renderer::tint(const color& value) noexcept {
        tint_ = value;
        return *this;
    }

    inline const color& sprite_renderer::tint() const noexcept {
        return tint_;
    }

    inline sprite_renderer& sprite_renderer::sprite(const sprite_asset::ptr& value) noexcept {
        sprite_ = value;
        return *this;
    }

    inline const sprite_asset::ptr& sprite_renderer::sprite() const noexcept {
        return sprite_;
    }

    inline render::property_block& sprite_renderer::properties() noexcept {
        return properties_;
    }

    inline const render::property_block& sprite_renderer::properties() const noexcept {
        return properties_;
    }
}
