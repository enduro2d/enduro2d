/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "quat.hpp"
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
        quat<T> rotation = quat<T>::identity();
        vec3<T> scale = vec3<T>::unit();
    public:
        static const trs3& zero() noexcept;
        static const trs3& identity() noexcept;
    public:
        trs3() noexcept = default;
        trs3(const trs3& other) noexcept = default;
        trs3& operator=(const trs3& other) noexcept = default;

        trs3(const vec3<T>& t,
             const quat<T>& r,
             const vec3<T>& s) noexcept;

        template < typename To >
        trs3<To> cast_to() const noexcept;
    };
}

namespace e2d
{
    template < typename T >
    const trs3<T>& trs3<T>::zero() noexcept {
        static const trs3<T> zero{
            vec3<T>::zero(),
            quat<T>::zero(),
            vec3<T>::zero()};
        return zero;
    }

    template < typename T >
    const trs3<T>& trs3<T>::identity() noexcept {
        static const trs3<T> identity{
            vec3<T>::zero(),
            quat<T>::identity(),
            vec3<T>::unit()};
        return identity;
    }

    template < typename T >
    trs3<T>::trs3(
        const vec3<T>& t,
        const quat<T>& r,
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
    trs3<T> make_trs3(
        const vec3<T>& t,
        const quat<T>& r,
        const vec3<T>& s) noexcept
    {
        return trs3<T>(t, r, s);
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
        return trs3<T>(t, quat<T>::identity(), vec3<T>::unit());
    }

    template < typename T >
    trs3<T> make_rotation_trs3(const quat<T>& r) noexcept {
        return trs3<T>(vec3<T>::zero(), r, vec3<T>::unit());
    }

    template < typename T >
    trs3<T> make_scale_trs3(const vec3<T>& s) noexcept {
        return trs3<T>(vec3<T>::zero(), quat<T>::identity(), s);
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

    template < typename T >
    bool contains_nan(const trs3<T>& v) noexcept {
        return contains_nan(v.translation)
            || contains_nan(v.rotation)
            || contains_nan(v.scale);
    }
}
