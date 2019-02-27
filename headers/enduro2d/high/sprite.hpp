/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/texture_asset.hpp"
#include "assets/material_asset.hpp"

namespace e2d
{
    class sprite final {
    public:
        sprite() = default;

        sprite(sprite&& other) noexcept;
        sprite& operator=(sprite&& other) noexcept;

        sprite(const sprite& other);
        sprite& operator=(const sprite& other);

        void clear() noexcept;
        void swap(sprite& other) noexcept;

        sprite& assign(sprite&& other) noexcept;
        sprite& assign(const sprite& other);

        sprite& set_size(const v2f& size) noexcept;
        sprite& set_pivot(const v2f& pivot) noexcept;
        sprite& set_texrect(const b2f& texrect) noexcept;
        sprite& set_texture(const texture_asset::ptr& texture) noexcept;
        sprite& set_material(const material_asset::ptr& material) noexcept;

        const v2f& size() const noexcept;
        const v2f& pivot() const noexcept;
        const b2f& texrect() const noexcept;
        const texture_asset::ptr& texture() const noexcept;
        const material_asset::ptr& material() const noexcept;
    private:
        v2f size_;
        v2f pivot_;
        b2f texrect_;
        texture_asset::ptr texture_;
        material_asset::ptr material_;
    };

    void swap(sprite& l, sprite& r) noexcept;
    bool operator==(const sprite& l, const sprite& r) noexcept;
    bool operator!=(const sprite& l, const sprite& r) noexcept;
}
