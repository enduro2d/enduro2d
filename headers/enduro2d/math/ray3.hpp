/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "vec3.hpp"

namespace e2d
{
    template < typename T >
    class ray3 final {
        static_assert(
            std::is_arithmetic_v<T>,
            "type of 'ray3' must be arithmetic");
    public:
        using self_type = ray3;
        using value_type = T;
    public:
        vec3<T> origin;
        vec3<T> direction;
    public:
        static constexpr ray3 zero() noexcept;
        static constexpr ray3 unit_x() noexcept;
        static constexpr ray3 unit_y() noexcept;
        static constexpr ray3 unit_z() noexcept;
    public:
        constexpr ray3() noexcept = default;
        constexpr ray3(const ray3& other) noexcept = default;
        constexpr ray3& operator=(const ray3& other) noexcept = default;

        constexpr ray3(T dx, T dy, T dz) noexcept;
        constexpr ray3(T ox, T oy, T oz, T dx, T dy, T dz) noexcept;

        constexpr ray3(const vec3<T>& direction) noexcept;
        constexpr ray3(const vec3<T>& origin, const vec3<T>& direction) noexcept;

        template < typename To >
        ray3<To> cast_to() const noexcept;

        T* data() noexcept;
        const T* data() const noexcept;

        T& operator[](std::size_t index) noexcept;
        T operator[](std::size_t index) const noexcept;

        ray3& operator+=(T v) noexcept;
        ray3& operator-=(T v) noexcept;
        ray3& operator*=(T v) noexcept;
        ray3& operator/=(T v) noexcept;

        ray3& operator+=(const vec3<T>& v) noexcept;
        ray3& operator-=(const vec3<T>& v) noexcept;
        ray3& operator*=(const vec3<T>& v) noexcept;
        ray3& operator/=(const vec3<T>& v) noexcept;
    };
}

namespace e2d
{
    template < typename T >
    constexpr ray3<T> ray3<T>::zero() noexcept {
        return {vec3<T>::zero(), vec3<T>::zero()};
    }

    template < typename T >
    constexpr ray3<T> ray3<T>::unit_x() noexcept {
        return {vec3<T>::zero(), vec3<T>::unit_x()};
    }

    template < typename T >
    constexpr ray3<T> ray3<T>::unit_y() noexcept {
        return {vec3<T>::zero(), vec3<T>::unit_y()};
    }

    template < typename T >
    constexpr ray3<T> ray3<T>::unit_z() noexcept {
        return {vec3<T>::zero(), vec3<T>::unit_z()};
    }

    template < typename T >
    constexpr ray3<T>::ray3(T dx, T dy, T dz) noexcept
    : direction(dx, dy, dz) {}

    template < typename T >
    constexpr ray3<T>::ray3(T ox, T oy, T oz, T dx, T dy, T dz) noexcept
    : origin(ox, oy, oz)
    , direction(dx, dy, dz) {}

    template < typename T >
    constexpr ray3<T>::ray3(const vec3<T>& direction) noexcept
    : direction(direction) {}

    template < typename T >
    constexpr ray3<T>::ray3(const vec3<T>& origin, const vec3<T>& direction) noexcept
    : origin(origin)
    , direction(direction) {}

    template < typename T >
    template < typename To >
    ray3<To> ray3<T>::cast_to() const noexcept {
        return {
            origin.template cast_to<To>(),
            direction.template cast_to<To>()};
    }

    template < typename T >
    T* ray3<T>::data() noexcept {
        return origin.data();
    }

    template < typename T >
    const T* ray3<T>::data() const noexcept {
        return origin.data();
    }

    template < typename T >
    T& ray3<T>::operator[](std::size_t index) noexcept {
        E2D_ASSERT(index < 6);
        return data()[index];
    }

    template < typename T >
    T ray3<T>::operator[](std::size_t index) const noexcept {
        E2D_ASSERT(index < 6);
        return data()[index];
    }

    template < typename T >
    ray3<T>& ray3<T>::operator+=(T v) noexcept {
        origin += v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator-=(T v) noexcept {
        origin -= v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator*=(T v) noexcept {
        direction *= v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator/=(T v) noexcept {
        direction /= v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator+=(const vec3<T>& v) noexcept {
        origin += v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator-=(const vec3<T>& v) noexcept {
        origin -= v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator*=(const vec3<T>& v) noexcept {
        direction *= v;
        return *this;
    }

    template < typename T >
    ray3<T>& ray3<T>::operator/=(const vec3<T>& v) noexcept {
        direction /= v;
        return *this;
    }
}

namespace e2d
{
    //
    // make_ray3
    //

    template < typename T >
    constexpr ray3<T> make_ray3(T dx, T dy, T dz) noexcept {
        return {dx, dy, dz};
    }

    template < typename T >
    constexpr ray3<T> make_ray3(T ox, T oy, T oz, T dx, T dy, T dz) noexcept {
        return {ox, oy, oz, dx, dy, dz};
    }

    template < typename T >
    constexpr ray3<T> make_ray3(const vec3<T>& direction) noexcept {
        return {direction};
    }

    template < typename T >
    constexpr ray3<T> make_ray3(const vec3<T>& origin, const vec3<T>& direction) noexcept {
        return {origin, direction};
    }

    //
    // ray3 (==,!=) ray3
    //

    template < typename T >
    bool operator==(const ray3<T>& l, const ray3<T>& r) noexcept {
        return l.origin == r.origin
            && l.direction == r.direction;
    }

    template < typename T >
    bool operator!=(const ray3<T>& l, const ray3<T>& r) noexcept {
        return !(l == r);
    }

    //
    // ray3 (+,-,*,/) value
    //

    template < typename T >
    ray3<T> operator+(const ray3<T>& l, T v) noexcept {
        return {
            l.origin + v,
            l.direction};
    }

    template < typename T >
    ray3<T> operator-(const ray3<T>& l, T v) noexcept {
        return {
            l.origin - v,
            l.direction};
    }

    template < typename T >
    ray3<T> operator*(const ray3<T>& l, T v) noexcept {
        return {
            l.origin,
            l.direction * v};
    }

    template < typename T >
    ray3<T> operator/(const ray3<T>& l, T v) noexcept {
        return {
            l.origin,
            l.direction / v};
    }

    //
    // ray3 (+,-,*,/) vec3
    //

    template < typename T >
    ray3<T> operator+(const ray3<T>& l, const vec3<T>& v) noexcept {
        return {
            l.origin + v,
            l.direction};
    }

    template < typename T >
    ray3<T> operator-(const ray3<T>& l, const vec3<T>& v) noexcept {
        return {
            l.origin - v,
            l.direction};
    }

    template < typename T >
    ray3<T> operator*(const ray3<T>& l, const vec3<T>& v) noexcept {
        return {
            l.origin,
            l.direction * v};
    }

    template < typename T >
    ray3<T> operator/(const ray3<T>& l, const vec3<T>& v) noexcept {
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
        const ray3<T>& l,
        const ray3<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.origin, r.origin, precision)
            && math::approximately(l.direction, r.direction, precision);
    }
}
