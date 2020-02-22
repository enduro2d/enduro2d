/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/sprite.hpp>

namespace e2d
{
    sprite::sprite(sprite&& other) noexcept {
        assign(std::move(other));
    }

    sprite& sprite::operator=(sprite&& other) noexcept {
        return assign(std::move(other));
    }

    sprite::sprite(const sprite& other) {
        assign(other);
    }

    sprite& sprite::operator=(const sprite& other) {
        return assign(other);
    }

    void sprite::clear() noexcept {
        pivot_ = v2f::zero();
        inner_texrect_ = b2f::zero();
        outer_texrect_ = b2f::zero();
        texture_.reset();
    }

    void sprite::swap(sprite& other) noexcept {
        using std::swap;
        swap(pivot_, other.pivot_);
        swap(inner_texrect_, other.inner_texrect_);
        swap(outer_texrect_, other.outer_texrect_);
        swap(texture_, other.texture_);
    }

    sprite& sprite::assign(sprite&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    sprite& sprite::assign(const sprite& other) {
        if ( this != &other ) {
            sprite s;
            s.pivot_ = other.pivot_;
            s.inner_texrect_ = other.inner_texrect_;
            s.outer_texrect_ = other.outer_texrect_;
            s.texture_ = other.texture_;
            swap(s);
        }
        return *this;
    }

    sprite& sprite::set_pivot(const v2f& pivot) noexcept {
        pivot_ = pivot;
        return *this;
    }

    sprite& sprite::set_inner_texrect(const b2f& texrect) noexcept {
        inner_texrect_ = texrect;
        return *this;
    }

    sprite& sprite::set_outer_texrect(const b2f& texrect) noexcept {
        outer_texrect_ = texrect;
        return *this;
    }

    sprite& sprite::set_texture(const texture_asset::ptr& texture) noexcept {
        texture_ = texture;
        return *this;
    }

    const v2f& sprite::pivot() const noexcept {
        return pivot_;
    }

    const b2f& sprite::inner_texrect() const noexcept {
        return inner_texrect_;
    }

    const b2f& sprite::outer_texrect() const noexcept {
        return outer_texrect_;
    }

    const texture_asset::ptr& sprite::texture() const noexcept {
        return texture_;
    }
}

namespace e2d
{
    void swap(sprite& l, sprite& r) noexcept {
        l.swap(r);
    }

    bool operator==(const sprite& l, const sprite& r) noexcept {
        return l.pivot() == r.pivot()
            && l.inner_texrect() == r.inner_texrect()
            && l.outer_texrect() == r.outer_texrect()
            && l.texture() == r.texture();
    }

    bool operator!=(const sprite& l, const sprite& r) noexcept {
        return !(l == r);
    }
}
