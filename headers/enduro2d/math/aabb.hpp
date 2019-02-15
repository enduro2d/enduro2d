/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "vec3.hpp"

namespace e2d
{
    template < typename T >
    class aabb final {
        static_assert(
            std::is_arithmetic<T>::value,
            "type of 'aabb' must be arithmetic");
    public:
        using self_type = aabb;
        using value_type = T;
    public:
        vec3<T> position;
        vec3<T> size;
    public:
        aabb() noexcept = default;
        aabb(const aabb& other) noexcept = default;
        aabb& operator=(const aabb& other) noexcept = default;

        aabb(T w, T h, T l) noexcept;
        aabb(T x, T y, T z, T w, T h, T l) noexcept;

        aabb(const vec3<T>& nsize) noexcept;
        aabb(const vec3<T>& nposition, const vec3<T>& nsize) noexcept;

        template < typename To >
        aabb<To> cast_to() const noexcept;

        T* data() noexcept;
        const T* data() const noexcept;

        T& operator[](std::size_t index) noexcept;
        T  operator[](std::size_t index) const noexcept;

        aabb& operator+=(T v) noexcept;
        aabb& operator-=(T v) noexcept;
        aabb& operator*=(T v) noexcept;
        aabb& operator/=(T v) noexcept;

        aabb& operator+=(const vec3<T>& v) noexcept;
        aabb& operator-=(const vec3<T>& v) noexcept;
        aabb& operator*=(const vec3<T>& v) noexcept;
        aabb& operator/=(const vec3<T>& v) noexcept;
    };
}

namespace e2d
{
    template < typename T >
    aabb<T>::aabb(T w, T h, T l) noexcept
    : size(w, h, l) {}

    template < typename T >
    aabb<T>::aabb(T x, T y, T z, T w, T h, T l) noexcept
    : position(x, y, z)
    , size(w, h, l) {}

    template < typename T >
    aabb<T>::aabb(const vec3<T>& nsize) noexcept
    : size(nsize) {}

    template < typename T >
    aabb<T>::aabb(const vec3<T>& nposition, const vec3<T>& nsize) noexcept
    : position(nposition)
    , size(nsize) {}

    template < typename T >
    template < typename To >
    aabb<To> aabb<T>::cast_to() const noexcept {
        return {
            position.template cast_to<To>(),
            size.template cast_to<To>()};
    }

    template < typename T >
    T* aabb<T>::data() noexcept {
        return position.data();
    }

    template < typename T >
    const T* aabb<T>::data() const noexcept {
        return position.data();
    }

    template < typename T >
    T& aabb<T>::operator[](std::size_t index) noexcept {
        E2D_ASSERT(index < 6);
        return data()[index];
    }

    template < typename T >
    T aabb<T>::operator[](std::size_t index) const noexcept {
        E2D_ASSERT(index < 6);
        return data()[index];
    }

    template < typename T >
    aabb<T>& aabb<T>::operator+=(T v) noexcept {
        position += v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator-=(T v) noexcept {
        position -= v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator*=(T v) noexcept {
        size *= v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator/=(T v) noexcept {
        size /= v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator+=(const vec3<T>& v) noexcept {
        position += v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator-=(const vec3<T>& v) noexcept {
        position -= v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator*=(const vec3<T>& v) noexcept {
        size *= v;
        return *this;
    }

    template < typename T >
    aabb<T>& aabb<T>::operator/=(const vec3<T>& v) noexcept {
        size /= v;
        return *this;
    }
}

namespace e2d
{
    //
    // make_aabb
    //

    template < typename T >
    aabb<T> make_aabb(T w, T h, T l) noexcept {
        return {w, h, l};
    }

    template < typename T >
    aabb<T> make_aabb(T x, T y, T z, T w, T h, T l) noexcept {
        return {x, y, z, w, h, l};
    }

    template < typename T >
    aabb<T> make_aabb(const vec3<T>& size) noexcept {
        return {size};
    }

    template < typename T >
    aabb<T> make_aabb(const vec3<T>& position, const vec3<T>& size) noexcept {
        return {position, size};
    }

    //
    // aabb (==,!=) aabb
    //

    template < typename T >
    bool operator==(const aabb<T>& l, const aabb<T>& r) noexcept {
        return l.position == r.position
            && l.size == r.size;
    }

    template < typename T >
    bool operator!=(const aabb<T>& l, const aabb<T>& r) noexcept {
        return !(l == r);
    }

    //
    // aabb (<,>,<=,>=) aabb
    //

    template < typename T >
    bool operator<(const aabb<T>& l, const aabb<T>& r) noexcept {
        return l.size.x * l.size.y * l.size.z < r.size.x * r.size.y * r.size.z;
    }

    template < typename T >
    bool operator>(const aabb<T>& l, const aabb<T>& r) noexcept {
        return r < l;
    }

    template < typename T >
    bool operator<=(const aabb<T>& l, const aabb<T>& r) noexcept {
        return !(r < l);
    }

    template < typename T >
    bool operator>=(const aabb<T>& l, const aabb<T>& r) noexcept {
        return !(l < r);
    }

    //
    // aabb (+,-,*,/) value
    //

    template < typename T >
    aabb<T> operator+(const aabb<T>& l, T v) noexcept {
        return {
            l.position + v,
            l.size};
    }

    template < typename T >
    aabb<T> operator-(const aabb<T>& l, T v) noexcept {
        return {
            l.position - v,
            l.size};
    }

    template < typename T >
    aabb<T> operator*(const aabb<T>& l, T v) noexcept {
        return {
            l.position,
            l.size * v};
    }

    template < typename T >
    aabb<T> operator/(const aabb<T>& l, T v) noexcept {
        return {
            l.position,
            l.size / v};
    }

    //
    // aabb (+,-,*,/) vec3
    //

    template < typename T >
    aabb<T> operator+(const aabb<T>& l, const vec3<T>& v) noexcept {
        return {
            l.position + v,
            l.size};
    }

    template < typename T >
    aabb<T> operator-(const aabb<T>& l, const vec3<T>& v) noexcept {
        return {
            l.position - v,
            l.size};
    }

    template < typename T >
    aabb<T> operator*(const aabb<T>& l, const vec3<T>& v) noexcept {
        return {
            l.position,
            l.size * v};
    }

    template < typename T >
    aabb<T> operator/(const aabb<T>& l, const vec3<T>& v) noexcept {
        return {
            l.position,
            l.size / v};
    }
}

namespace e2d { namespace math
{
    //
    // make_minmax_aabb
    //

    template < typename T >
    aabb<T> make_minmax_aabb(T x1, T y1, T z1, T x2, T y2, T z2) noexcept {
        const vec3<T> min = {math::min(x1, x2), math::min(y1, y2), math::min(z1, z2)};
        const vec3<T> max = {math::max(x1, x2), math::max(y1, y2), math::max(z1, z2)};
        return {min, max - min};
    }

    template < typename T >
    aabb<T> make_minmax_aabb(const vec3<T>& p1, const vec3<T>& p2) noexcept {
        return make_minmax_aabb(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
    }

    template < typename T >
    aabb<T> make_minmax_aabb(const aabb<T>& b) noexcept {
        return make_minmax_aabb(b.position, b.position + b.size);
    }

    //
    // approximately
    //

    template < typename T >
    bool approximately(
        const aabb<T>& l,
        const aabb<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.position, r.position, precision)
            && math::approximately(l.size, r.size, precision);
    }

    //
    // minimum/maximum
    //

    template < typename T >
    vec3<T> minimum(const aabb<T>& b) noexcept {
        return math::minimized(b.position, b.position + b.size);
    }

    template < typename T >
    vec3<T> maximum(const aabb<T>& b) noexcept {
        return math::maximized(b.position, b.position + b.size);
    }

    //
    // volume
    //

    template < typename T >
    T volume(const aabb<T>& b) noexcept {
        return b.size.x * b.size.y * b.size.z;
    }

    template < typename T >
    T abs_volume(const aabb<T>& b) noexcept {
        return math::abs(b.size.x * b.size.y * b.size.z);
    }

    //
    // merged
    //

    template < typename T >
    aabb<T> merged(const aabb<T>& l, const aabb<T>& r) noexcept {
        return make_minmax_aabb(
            math::minimized(minimum(l), minimum(r)),
            math::maximized(maximum(l), maximum(r)));
    }

    //
    // inside
    //

    template < typename T >
    bool inside(const aabb<T>& r, const vec3<T>& p) noexcept {
        const vec3<T> min = minimum(r);
        const vec3<T> max = maximum(r);
        return p.x >= min.x && p.x <= max.x
            && p.y >= min.y && p.y <= max.y
            && p.z >= min.z && p.z <= max.z;
    }

    //
    // overlaps
    //

    template < typename T >
    bool overlaps(const aabb<T>& l, const aabb<T>& r) noexcept {
        const vec3<T> min_l = minimum(l);
        const vec3<T> max_l = maximum(l);
        const vec3<T> min_r = minimum(r);
        const vec3<T> max_r = maximum(r);
        return max_l.x > min_r.x && min_l.x < max_r.x
            && max_l.y > min_r.y && min_l.y < max_r.y
            && max_l.z > min_r.z && min_l.z < max_r.z;
    }

    //
    // normalized_to_point/point_to_normalized
    //

    template < typename T >
    vec3<T> normalized_to_point(const aabb<T>& r, const vec3<T>& p) noexcept {
        const vec3<T> min = minimum(r);
        const vec3<T> max = maximum(r);
        return math::lerp(min, max, p);
    }

    template < typename T >
    vec3<T> point_to_normalized(const aabb<T>& r, const vec3<T>& p) noexcept {
        const vec3<T> min = minimum(r);
        const vec3<T> max = maximum(r);
        return math::inverse_lerp(min, max, p);
    }

    //
    // contains_nan
    //

    template < typename T >
    bool contains_nan(const aabb<T>& r) noexcept {
        return math::contains_nan(r.position)
            || math::contains_nan(r.size);
    }
}}
