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

    template < typename T >
    rect<T> make_minmax_rect(T x1, T y1, T x2, T y2) noexcept {
        const vec2<T> min = {math::min(x1, x2), math::min(y1, y2)};
        const vec2<T> max = {math::max(x1, x2), math::max(y1, y2)};
        return {min, max - min};
    }

    template < typename T >
    rect<T> make_minmax_rect(const vec2<T>& p1, const vec2<T>& p2) noexcept {
        return make_minmax_rect(p1.x, p1.y, p2.x, p2.y);
    }

    template < typename T >
    rect<T> make_minmax_rect(const rect<T>& r) noexcept {
        return make_minmax_rect(r.position, r.position + r.size);
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
        return make_minmax_rect(
            math::minimized(minimum(l), minimum(r)),
            math::maximized(maximum(l), maximum(r)));
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
    // overlaps
    //

    template < typename T >
    bool overlaps(const rect<T>& l, const rect<T>& r) noexcept {
        const vec2<T> min_l = minimum(l);
        const vec2<T> max_l = maximum(l);
        const vec2<T> min_r = minimum(r);
        const vec2<T> max_r = maximum(r);
        return max_l.x > min_r.x
            && min_l.x < max_r.x
            && max_l.y > min_r.y
            && min_l.y < max_r.y;
    }

    //
    // normalized_to_point/point_to_normalized
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point<T>::value, vec2<T>>
    normalized_to_point(const rect<T>& r, const vec2<T>& p) noexcept {
        const vec2<T> min = minimum(r);
        const vec2<T> max = maximum(r);
        return math::lerp(min, max, p);
    }

    template < typename T >
    std::enable_if_t<std::is_floating_point<T>::value, vec2<T>>
    point_to_normalized(const rect<T>& r, const vec2<T>& p) noexcept {
        const vec2<T> min = minimum(r);
        const vec2<T> max = maximum(r);
        return math::inverse_lerp(min, max, p);
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
