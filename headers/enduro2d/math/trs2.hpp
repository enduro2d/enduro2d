/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "trig.hpp"
#include "unit.hpp"
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
        rad<T> rotation = rad<T>::zero();
        vec2<T> scale = vec2<T>::unit();
    public:
        static constexpr trs2 zero() noexcept;
        static constexpr trs2 identity() noexcept;
    public:
        constexpr trs2() noexcept = default;
        constexpr trs2(const trs2& other) noexcept = default;
        constexpr trs2& operator=(const trs2& other) noexcept = default;

        template < typename AngleTag >
        constexpr trs2(
            const vec2<T>& t,
            const unit<T, AngleTag>& r,
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
            rad<T>::zero(),
            vec2<T>::zero()};
    }

    template < typename T >
    constexpr trs2<T> trs2<T>::identity() noexcept {
        return {
            vec2<T>::zero(),
            rad<T>::zero(),
            vec2<T>::unit()};
    }

    template < typename T >
    template < typename AngleTag >
    constexpr trs2<T>::trs2(
        const vec2<T>& t,
        const unit<T, AngleTag>& r,
        const vec2<T>& s) noexcept
    : translation(t)
    , rotation(r)
    , scale(s) {}

    template < typename T >
    template < typename To >
    trs2<To> trs2<T>::cast_to() const noexcept {
        return {
            translation.template cast_to<To>(),
            rotation.template cast_to<To>(),
            scale.template cast_to<To>()};
    }
}

namespace e2d
{
    //
    // make_trs2
    //

    template < typename T, typename AngleTag >
    constexpr trs2<T> make_trs2(
        const vec2<T>& t,
        const unit<T, AngleTag>& r,
        const vec2<T>& s) noexcept
    {
        return trs2<T>(t, r, s);
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
        return trs2<T>(t, rad<T>::zero(), vec2<T>::unit());
    }

    template < typename T, typename AngleTag >
    trs2<T> make_rotation_trs2(const unit<T, AngleTag>& r) noexcept {
        return trs2<T>(vec2<T>::zero(), r, vec2<T>::unit());
    }

    template < typename T >
    trs2<T> make_scale_trs2(const vec2<T>& s) noexcept {
        return trs2<T>(vec2<T>::zero(), rad<T>::zero(), s);
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

    template < typename T >
    bool contains_nan(const trs2<T>& v) noexcept {
        return contains_nan(v.translation)
            || contains_nan(v.rotation)
            || contains_nan(v.scale);
    }
}
