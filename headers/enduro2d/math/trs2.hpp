/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "vec2.hpp"

namespace e2d
{
    template < typename T >
    class trs2 final {
        static_assert(
            std::is_arithmetic_v<T>,
            "type of 'trs2' must be arithmetic");
    public:
        using self_type = trs2;
        using value_type = T;
    public:
        vec2<T> translation = vec2<T>::zero();
        T rotation = T(0);
        vec2<T> scale = vec2<T>::unit();
    public:
        static constexpr trs2 zero() noexcept;
        static constexpr trs2 identity() noexcept;
    public:
        constexpr trs2() noexcept = default;
        constexpr trs2(const trs2& other) noexcept = default;
        constexpr trs2& operator=(const trs2& other) noexcept = default;

        constexpr trs2(
            const vec2<T>& t,
            const T& r,
            const vec2<T>& s) noexcept;

        template < typename To >
        trs2<To> cast_to() const noexcept;
    };
}

namespace e2d
{
    template < typename T >
    constexpr trs2<T> trs2<T>::zero() noexcept {
        return {
            vec2<T>::zero(),
            T(0),
            vec2<T>::zero()};
    }

    template < typename T >
    constexpr trs2<T> trs2<T>::identity() noexcept {
        return {
            vec2<T>::zero(),
            T(0),
            vec2<T>::unit()};
    }

    template < typename T >
    constexpr trs2<T>::trs2(
        const vec2<T>& t,
        const T& r,
        const vec2<T>& s) noexcept
    : translation(t)
    , rotation(r)
    , scale(s) {}

    template < typename T >
    template < typename To >
    trs2<To> trs2<T>::cast_to() const noexcept {
        return {
            translation.template cast_to<To>(),
            math::numeric_cast<To>(rotation),
            scale.template cast_to<To>()};
    }
}

namespace e2d
{
    //
    // make_trs2
    //

    template < typename T >
    constexpr trs2<T> make_trs2(
        const vec2<T>& t,
        const T& r,
        const vec2<T>& s) noexcept
    {
        return { t, r, s };
    }

    //
    // trs2 (==,!=) trs2
    //

    template < typename T >
    bool operator==(const trs2<T>& l, const trs2<T>& r) noexcept {
        return l.translation == r.translation
            && l.rotation == r.rotation
            && l.scale == r.scale;
    }

    template < typename T >
    bool operator!=(const trs2<T>& l, const trs2<T>& r) noexcept {
        return !(l == r);
    }
}

namespace e2d::math
{
    template < typename T >
    trs2<T> make_translation_trs2(const vec2<T>& t) noexcept {
        return { t, T(0), vec2<T>::unit() };
    }

    template < typename T >
    trs2<T> make_rotation_trs2(const T& r) noexcept {
        return { vec2<T>::zero(), r, vec2<T>::unit() };
    }

    template < typename T >
    trs2<T> make_scale_trs2(const vec2<T>& s) noexcept {
        return { vec2<T>::zero(), T(0), s };
    }

    template < typename T >
    bool approximately(
        const trs2<T>& l,
        const trs2<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.translation, r.translation, precision)
            && math::approximately(l.rotation, r.rotation, precision)
            && math::approximately(l.scale, r.scale, precision);
    }
}
