/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/flipbook.hpp>

namespace e2d
{
    flipbook::flipbook() = default;
    flipbook::~flipbook() noexcept = default;

    flipbook::flipbook(flipbook&& other) noexcept {
        assign(std::move(other));
    }

    flipbook& flipbook::operator=(flipbook&& other) noexcept {
        return assign(std::move(other));
    }

    flipbook::flipbook(const flipbook& other) {
        assign(other);
    }

    flipbook& flipbook::operator=(const flipbook& other) {
        return assign(other);
    }

    void flipbook::clear() noexcept {
    }

    void flipbook::swap(flipbook& other) noexcept {
        using std::swap;
        E2D_UNUSED(other);
    }

    flipbook& flipbook::assign(flipbook&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    flipbook& flipbook::assign(const flipbook& other) {
        if ( this != &other ) {
            flipbook s;
            swap(s);
        }
        return *this;
    }
}

namespace e2d
{
    void swap(flipbook& l, flipbook& r) noexcept {
        l.swap(r);
    }

    bool operator==(const flipbook& l, const flipbook& r) noexcept {
        E2D_UNUSED(l, r);
        return true;
    }

    bool operator!=(const flipbook& l, const flipbook& r) noexcept {
        return !(l == r);
    }
}
