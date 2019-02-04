/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/components/sprite.hpp>

namespace e2d
{
    sprite& sprite::tint(const color& value) noexcept {
        tint_ = value;
        return *this;
    }

    sprite& sprite::texture(const texture_ptr& value) noexcept {
        texture_ = value;
        return *this;
    }

    sprite& sprite::texture_rect(const b2f& value) noexcept {
        texture_rect_ = value;
        return *this;
    }

    const color& sprite::tint() const noexcept {
        return tint_;
    }

    const texture_ptr& sprite::texture() const noexcept {
        return texture_;
    }

    const b2f& sprite::texture_rect() const noexcept {
        return texture_rect_;
    }
}
