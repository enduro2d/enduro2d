/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/prefab.hpp>

namespace e2d
{
    prefab::prefab() = default;
    prefab::~prefab() noexcept = default;

    prefab::prefab(prefab&& other) noexcept {
        assign(std::move(other));
    }

    prefab& prefab::operator=(prefab&& other) noexcept {
        return assign(std::move(other));
    }

    prefab::prefab(const prefab& other) {
        assign(other);
    }

    prefab& prefab::operator=(const prefab& other) {
        return assign(other);
    }

    void prefab::clear() noexcept {
    }

    void prefab::swap(prefab& other) noexcept {
        using std::swap;
        E2D_UNUSED(other);
    }

    prefab& prefab::assign(prefab&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    prefab& prefab::assign(const prefab& other) {
        if ( this != &other ) {
            prefab s;
            swap(s);
        }
        return *this;
    }
}

namespace e2d
{
    void swap(prefab& l, prefab& r) noexcept {
        l.swap(r);
    }

    bool operator==(const prefab& l, const prefab& r) noexcept {
        E2D_UNUSED(l, r);
        return true;
    }

    bool operator!=(const prefab& l, const prefab& r) noexcept {
        return !(l == r);
    }
}
