/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "vec2.hpp"

namespace e2d
{
    template < typename T >
    class rect final {
        static_assert(
            std::is_arithmetic<T>::value,
            "type of 'rect' must be arithmetic");
    public:
        using self_type = rect;
        using value_type = T;
    public:
        vec2<T> position;
        vec2<T> size;
    public:
        rect() noexcept = default;
        rect(const rect& other) noexcept = default;
        rect& operator=(const rect& other) noexcept = default;

        rect(T w, T h) noexcept;
        rect(T x, T y, T w, T h) noexcept;

        rect(const vec2<T>& nsize) noexcept;
        rect(const vec2<T>& nposition, const vec2<T>& nsize) noexcept;

        template < typename To >
        rect<To> cast_to() const noexcept;
    };
}

namespace e2d
{
    template < typename T >
    rect<T>::rect(T w, T h) noexcept
    : size(w, h) {}

    template < typename T >
    rect<T>::rect(T x, T y, T w, T h) noexcept
    : position(x, y)
    , size(w, h) {}

    template < typename T >
    rect<T>::rect(const vec2<T>& nsize) noexcept
    : size(nsize) {}

    template < typename T >
    rect<T>::rect(const vec2<T>& nposition, const vec2<T>& nsize) noexcept
    : position(nposition)
    , size(nsize) {}

    template < typename T >
    template < typename To >
    rect<To> rect<T>::cast_to() const noexcept {
        return {
            position.template cast_to<To>(),
            size.template cast_to<To>()};
    }
}

namespace e2d
{
    //
    // make_rect
    //

    template < typename T >
    rect<T> make_rect(T w, T h) noexcept {
        return {w, h};
    }

    template < typename T >
    rect<T> make_rect(T x, T y, T w, T h) noexcept {
        return {x, y, w, h};
    }

    template < typename T >
    rect<T> make_rect(const vec2<T>& size) noexcept {
        return {size};
    }

    template < typename T >
    rect<T> make_rect(const vec2<T>& position, const vec2<T>& size) noexcept {
        return {position, size};
    }

    //
    // rect (==,!=) rect
    //

    template < typename T >
    bool operator==(const rect<T>& l, const rect<T>& r) noexcept {
        return l.position == r.position
            && l.size == r.size;
    }

    template < typename T >
    bool operator!=(const rect<T>& l, const rect<T>& r) noexcept {
        return !(l == r);
    }

    //
    // rect (<,>,<=,>=) rect
    //

    template < typename T >
    bool operator<(const rect<T>& l, const rect<T>& r) noexcept {
        return l.size.x * l.size.y < r.size.x * r.size.y;
    }

    template < typename T >
    bool operator>(const rect<T>& l, const rect<T>& r) noexcept {
        return r < l;
    }

    template < typename T >
    bool operator<=(const rect<T>& l, const rect<T>& r) noexcept {
        return !(r < l);
    }

    template < typename T >
    bool operator>=(const rect<T>& l, const rect<T>& r) noexcept {
        return !(l < r);
    }
}

namespace e2d { namespace math
{
    //
    // approximately
    //

    template < typename T >
    bool approximately(
        const rect<T>& l,
        const rect<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.position, r.position, precision)
            && math::approximately(l.size, r.size, precision);
    }

    //
    // minimum/maximum
    //

    template < typename T >
    vec2<T> minimum(const rect<T>& r) noexcept {
        return math::minimized(r.position, r.position + r.size);
    }

    template < typename T >
    vec2<T> maximum(const rect<T>& r) noexcept {
        return math::maximized(r.position, r.position + r.size);
    }

    //
    // area
    //

    template < typename T >
    T area(const rect<T>& r) noexcept {
        return r.size.x * r.size.y;
    }

    template < typename T >
    T abs_area(const rect<T>& r) noexcept {
        return math::abs(r.size.x * r.size.y);
    }

    //
    // merged
    //

    template < typename T >
    rect<T> merged(const rect<T>& l, const rect<T>& r) noexcept {
        const vec2<T> min = math::minimized(minimum(l), minimum(r));
        const vec2<T> max = math::maximized(maximum(l), maximum(r));
        return { min, max - min };
    }

    //
    // inside
    //

    template < typename T >
    bool inside(const rect<T>& r, const vec2<T>& p) noexcept {
        const vec2<T> min = minimum(r);
        const vec2<T> max = maximum(r);
        return p.x >= min.x && p.x <= max.x
            && p.y >= min.y && p.y <= max.y;
    }

    //
    // contains_nan
    //

    template < typename T >
    bool contains_nan(const rect<T>& r) noexcept {
        return math::contains_nan(r.position)
            || math::contains_nan(r.size);
    }
}}
