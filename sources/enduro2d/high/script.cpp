/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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

    script::script(sol::protected_function&& func) noexcept {
        assign(std::move(func));
    }

    script::script(const sol::protected_function& func) {
        assign(func);
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
            func_ = other.func_;
        }
        return *this;
    }

    script& script::assign(sol::protected_function&& func) noexcept {
        func_ = std::move(func);
        return *this;
    }

    script& script::assign(const sol::protected_function& func) {
        func_ = func;
        return *this;
    }

    void script::clear() noexcept {
        func_.reset();
    }

    void script::swap(script& other) noexcept {
        using std::swap;
        swap(func_, other.func_);
    }

    bool script::empty() const noexcept {
        return !func_;
    }
}

namespace e2d
{
    void swap(script& l, script& r) noexcept {
        l.swap(r);
    }
}
