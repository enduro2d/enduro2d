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
    class trs3 final {
        static_assert(
            std::is_arithmetic_v<T>,
            "type of 'trs3' must be arithmetic");
    public:
        using self_type = trs3;
        using value_type = T;
    public:
        vec3<T> translation = vec3<T>::zero();
        vec3<T> rotation = vec3<T>::zero();
        vec3<T> scale = vec3<T>::unit();
    public:
        static constexpr trs3 zero() noexcept;
        static constexpr trs3 identity() noexcept;
    public:
        constexpr trs3() noexcept = default;
        constexpr trs3(const trs3& other) noexcept = default;
        constexpr trs3& operator=(const trs3& other) noexcept = default;

        constexpr trs3(
            const vec3<T>& t,
            const vec3<T>& r,
            const vec3<T>& s) noexcept;

        template < typename To >
        trs3<To> cast_to() const noexcept;
    };
}

namespace e2d
{
    template < typename T >
    constexpr trs3<T> trs3<T>::zero() noexcept {
        return {
            vec3<T>::zero(),
            vec3<T>::zero(),
            vec3<T>::zero()};
    }

    template < typename T >
    constexpr trs3<T> trs3<T>::identity() noexcept {
        return {
            vec3<T>::zero(),
            vec3<T>::zero(),
            vec3<T>::unit()};
    }

    template < typename T >
    constexpr trs3<T>::trs3(
        const vec3<T>& t,
        const vec3<T>& r,
        const vec3<T>& s) noexcept
    : translation(t)
    , rotation(r)
    , scale(s) {}

    template < typename T >
    template < typename To >
    trs3<To> trs3<T>::cast_to() const noexcept {
        return {
            translation.template cast_to<To>(),
            rotation.template cast_to<To>(),
            scale.template cast_to<To>()};
    }
}

namespace e2d
{
    //
    // make_trs3
    //

    template < typename T >
    constexpr trs3<T> make_trs3(
        const vec3<T>& t,
        const vec3<T>& r,
        const vec3<T>& s) noexcept
    {
        return { t, r, s };
    }

    //
    // trs3 (==,!=) trs3
    //

    template < typename T >
    bool operator==(const trs3<T>& l, const trs3<T>& r) noexcept {
        return l.translation == r.translation
            && l.rotation == r.rotation
            && l.scale == r.scale;
    }

    template < typename T >
    bool operator!=(const trs3<T>& l, const trs3<T>& r) noexcept {
        return !(l == r);
    }
}

namespace e2d::math
{
    template < typename T >
    trs3<T> make_translation_trs3(const vec3<T>& t) noexcept {
        return { t, vec3<T>::zero(), vec3<T>::unit() };
    }

    template < typename T >
    trs3<T> make_rotation_trs3(const vec3<T>& r) noexcept {
        return { vec3<T>::zero(), r, vec3<T>::unit() };
    }

    template < typename T >
    trs3<T> make_scale_trs3(const vec3<T>& s) noexcept {
        return { vec3<T>::zero(), vec3<T>::zero(), s };
    }

    template < typename T >
    bool approximately(
        const trs3<T>& l,
        const trs3<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.translation, r.translation, precision)
            && math::approximately(l.rotation, r.rotation, precision)
            && math::approximately(l.scale, r.scale, precision);
    }
}
