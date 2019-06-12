/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/script.hpp>

namespace e2d
{
    script::script(script&& other) noexcept {
        assign(std::move(other));
    }

    script& script::operator=(script&& other) noexcept {
        return assign(std::move(other));
    }

    script::script(const script& other) {
        assign(other);
    }

    script& script::operator=(const script& other) {
        return assign(other);
    }

    void script::clear() noexcept {
    }

    void script::swap(script& other) noexcept {
        using std::swap;
    }

    script& script::assign(script&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    script& script::assign(const script& other) {
        if ( this != &other ) {
            script s;
            swap(s);
        }
        return *this;
    }
}

namespace e2d
{
    void swap(script& l, script& r) noexcept {
        l.swap(r);
    }

    bool operator==(const script& l, const script& r) noexcept {
        return true;
    }

    bool operator!=(const script& l, const script& r) noexcept {
        return !(l == r);
    }
}
