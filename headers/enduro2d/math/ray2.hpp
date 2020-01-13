/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "vec2.hpp"

namespace e2d
{
    template < typename T >
    class ray2 final {
        static_assert(
            std::is_arithmetic_v<T>,
            "type of 'ray2' must be arithmetic");
    public:
        using self_type = ray2;
        using value_type = T;
    public:
        vec2<T> origin;
        vec2<T> direction;
    public:
        static constexpr ray2 zero() noexcept;
        static constexpr ray2 unit_x() noexcept;
        static constexpr ray2 unit_y() noexcept;
    public:
        constexpr ray2() noexcept = default;
        constexpr ray2(const ray2& other) noexcept = default;
        constexpr ray2& operator=(const ray2& other) noexcept = default;

        constexpr ray2(T dx, T dy) noexcept;
        constexpr ray2(T ox, T oy, T dx, T dy) noexcept;

        constexpr ray2(const vec2<T>& direction) noexcept;
        constexpr ray2(const vec2<T>& origin, const vec2<T>& direction) noexcept;

        template < typename To >
        ray2<To> cast_to() const noexcept;

        T* data() noexcept;
        const T* data() const noexcept;

        T& operator[](std::size_t index) noexcept;
        T operator[](std::size_t index) const noexcept;

        ray2& operator+=(T v) noexcept;
        ray2& operator-=(T v) noexcept;
        ray2& operator*=(T v) noexcept;
        ray2& operator/=(T v) noexcept;

        ray2& operator+=(const vec2<T>& v) noexcept;
        ray2& operator-=(const vec2<T>& v) noexcept;
        ray2& operator*=(const vec2<T>& v) noexcept;
        ray2& operator/=(const vec2<T>& v) noexcept;
    };
}

namespace e2d
{
    template < typename T >
    constexpr ray2<T> ray2<T>::zero() noexcept {
        return {vec2<T>::zero(), vec2<T>::zero()};
    }

    template < typename T >
    constexpr ray2<T> ray2<T>::unit_x() noexcept {
        return {vec2<T>::zero(), vec2<T>::unit_x()};
    }

    template < typename T >
    constexpr ray2<T> ray2<T>::unit_y() noexcept {
        return {vec2<T>::zero(), vec2<T>::unit_y()};
    }

    template < typename T >
    constexpr ray2<T>::ray2(T dx, T dy) noexcept
    : direction(dx, dy) {}

    template < typename T >
    constexpr ray2<T>::ray2(T ox, T oy, T dx, T dy) noexcept
    : origin(ox, oy)
    , direction(dx, dy) {}

    template < typename T >
    constexpr ray2<T>::ray2(const vec2<T>& direction) noexcept
    : direction(direction) {}

    template < typename T >
    constexpr ray2<T>::ray2(const vec2<T>& origin, const vec2<T>& direction) noexcept
    : origin(origin)
    , direction(direction) {}

    template < typename T >
    template < typename To >
    ray2<To> ray2<T>::cast_to() const noexcept {
        return {
            origin.template cast_to<To>(),
            direction.template cast_to<To>()};
    }

    template < typename T >
    T* ray2<T>::data() noexcept {
        return origin.data();
    }

    template < typename T >
    const T* ray2<T>::data() const noexcept {
        return origin.data();
    }

    template < typename T >
    T& ray2<T>::operator[](std::size_t index) noexcept {
        E2D_ASSERT(index < 4);
        return data()[index];
    }

    template < typename T >
    T ray2<T>::operator[](std::size_t index) const noexcept {
        E2D_ASSERT(index < 4);
        return data()[index];
    }

    template < typename T >
    ray2<T>& ray2<T>::operator+=(T v) noexcept {
        origin += v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator-=(T v) noexcept {
        origin -= v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator*=(T v) noexcept {
        direction *= v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator/=(T v) noexcept {
        direction /= v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator+=(const vec2<T>& v) noexcept {
        origin += v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator-=(const vec2<T>& v) noexcept {
        origin -= v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator*=(const vec2<T>& v) noexcept {
        direction *= v;
        return *this;
    }

    template < typename T >
    ray2<T>& ray2<T>::operator/=(const vec2<T>& v) noexcept {
        direction /= v;
        return *this;
    }
}

namespace e2d
{
    //
    // make_ray2
    //

    template < typename T >
    constexpr ray2<T> make_ray2(T dx, T dy) noexcept {
        return {dx, dy};
    }

    template < typename T >
    constexpr ray2<T> make_ray2(T ox, T oy, T dx, T dy) noexcept {
        return {ox, oy, dx, dy};
    }

    template < typename T >
    constexpr ray2<T> make_ray2(const vec2<T>& direction) noexcept {
        return {direction};
    }

    template < typename T >
    constexpr ray2<T> make_ray2(const vec2<T>& origin, const vec2<T>& direction) noexcept {
        return {origin, direction};
    }

    //
    // ray2 (==,!=) ray2
    //

    template < typename T >
    bool operator==(const ray2<T>& l, const ray2<T>& r) noexcept {
        return l.origin == r.origin
            && l.direction == r.direction;
    }

    template < typename T >
    bool operator!=(const ray2<T>& l, const ray2<T>& r) noexcept {
        return !(l == r);
    }

    //
    // ray2 (+,-,*,/) value
    //

    template < typename T >
    ray2<T> operator+(const ray2<T>& l, T v) noexcept {
        return {
            l.origin + v,
            l.direction};
    }

    template < typename T >
    ray2<T> operator-(const ray2<T>& l, T v) noexcept {
        return {
            l.origin - v,
            l.direction};
    }

    template < typename T >
    ray2<T> operator*(const ray2<T>& l, T v) noexcept {
        return {
            l.origin,
            l.direction * v};
    }

    template < typename T >
    ray2<T> operator/(const ray2<T>& l, T v) noexcept {
        return {
            l.origin,
            l.direction / v};
    }

    //
    // ray2 (+,-,*,/) vec2
    //

    template < typename T >
    ray2<T> operator+(const ray2<T>& l, const vec2<T>& v) noexcept {
        return {
            l.origin + v,
            l.direction};
    }

    template < typename T >
    ray2<T> operator-(const ray2<T>& l, const vec2<T>& v) noexcept {
        return {
            l.origin - v,
            l.direction};
    }

    template < typename T >
    ray2<T> operator*(const ray2<T>& l, const vec2<T>& v) noexcept {
        return {
            l.origin,
            l.direction * v};
    }

    template < typename T >
    ray2<T> operator/(const ray2<T>& l, const vec2<T>& v) noexcept {
        return {
            l.origin,
            l.direction / v};
    }
}

namespace e2d::math
{
    //
    // approximately
    //

    template < typename T >
    bool approximately(
        const ray2<T>& l,
        const ray2<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.origin, r.origin, precision)
            && math::approximately(l.direction, r.direction, precision);
    }
}
