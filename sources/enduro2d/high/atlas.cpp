/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/atlas.hpp>

namespace e2d
{
    atlas::atlas() = default;
    atlas::~atlas() noexcept = default;

    atlas::atlas(atlas&& other) noexcept {
        assign(std::move(other));
    }

    atlas& atlas::operator=(atlas&& other) noexcept {
        return assign(std::move(other));
    }

    atlas::atlas(const atlas& other) {
        assign(other);
    }

    atlas& atlas::operator=(const atlas& other) {
        return assign(other);
    }

    void atlas::clear() noexcept {
        texture_.reset();
    }

    void atlas::swap(atlas& other) noexcept {
        using std::swap;
        swap(texture_, other.texture_);
    }

    atlas& atlas::assign(atlas&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    atlas& atlas::assign(const atlas& other) {
        if ( this != &other ) {
            atlas s;
            s.texture_ = other.texture_;
            swap(s);
        }
        return *this;
    }

    atlas& atlas::set_texture(const texture_asset::ptr& texture) noexcept {
        texture_ = texture;
        return *this;
    }

    const texture_asset::ptr& atlas::texture() const noexcept {
        return texture_;
    }
}

namespace e2d
{
    void swap(atlas& l, atlas& r) noexcept {
        l.swap(r);
    }

    bool operator==(const atlas& l, const atlas& r) noexcept {
        return l.texture() == r.texture();
    }

    bool operator!=(const atlas& l, const atlas& r) noexcept {
        return !(l == r);
    }
}
