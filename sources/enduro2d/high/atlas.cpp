/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
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
    }

    void atlas::swap(atlas& other) noexcept {
        using std::swap;
        E2D_UNUSED(other);
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
            swap(s);
        }
        return *this;
    }
}

namespace e2d
{
    void swap(atlas& l, atlas& r) noexcept {
        l.swap(r);
    }

    bool operator==(const atlas& l, const atlas& r) noexcept {
        E2D_UNUSED(l, r);
        return true;
    }

    bool operator!=(const atlas& l, const atlas& r) noexcept {
        return !(l == r);
    }
}
