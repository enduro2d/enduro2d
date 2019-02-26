/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/sprite.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    sprite::sprite() = default;

    sprite::sprite(sprite&& other) noexcept = default;
    sprite& sprite::operator=(sprite&& other) noexcept = default;

    sprite::sprite(const sprite& other) = default;
    sprite& sprite::operator=(const sprite& other) = default;

    void sprite::swap(sprite& other) noexcept {
        using std::swap;
        swap(size_, other.size_);
        swap(pivot_, other.pivot_);
        swap(texrect_, other.texrect_);
        swap(texture_, other.texture_);
        swap(material_, other.material_);
    }

    sprite& sprite::assign(sprite&& other) noexcept {
        return *this = std::move(other);
    }

    sprite& sprite::assign(const sprite& other) {
        return *this = other;
    }

    sprite& sprite::set_size(const v2f& size) noexcept {
        size_ = size;
        return *this;
    }

    sprite& sprite::set_pivot(const v2f& pivot) noexcept {
        pivot_ = pivot;
        return *this;
    }

    sprite& sprite::set_texrect(const b2f& texrect) noexcept {
        texrect_ = texrect;
        return *this;
    }

    sprite& sprite::set_texture(const texture_asset::ptr& texture) noexcept {
        texture_ = texture;
        return *this;
    }

    sprite& sprite::set_material(const material_asset::ptr& material) noexcept {
        material_ = material;
        return *this;
    }

    const v2f& sprite::size() const noexcept {
        return size_;
    }

    const v2f& sprite::pivot() const noexcept {
        return pivot_;
    }

    const b2f& sprite::texrect() const noexcept {
        return texrect_;
    }

    const texture_asset::ptr& sprite::texture() const noexcept {
        return texture_;
    }

    const material_asset::ptr& sprite::material() const noexcept {
        return material_;
    }
}

namespace e2d
{
    void swap(sprite& l, sprite& r) noexcept {
        l.swap(r);
    }

    bool operator==(const sprite& l, const sprite& r) noexcept {
        return l.size() == r.size()
            && l.pivot() == r.pivot()
            && l.texrect() == r.texrect()
            && l.texture() == r.texture()
            && l.material() == r.material();
    }

    bool operator!=(const sprite& l, const sprite& r) noexcept {
        return !(l == r);
    }
}
