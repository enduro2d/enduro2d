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
#include "vec3.hpp"
#include "vec4.hpp"

namespace e2d
{
    template < typename T >
    class mat2 final {
        static_assert(
            std::is_arithmetic<T>::value,
            "type of 'mat2' must be arithmetic");
    public:
        using self_type = mat2;
        using value_type = T;
    public:
        vec2<T> rows[2] = {
            {1, 0},
            {0, 1}};
    public:
        static const mat2& zero() noexcept;
        static const mat2& identity() noexcept;
    public:
        mat2() noexcept = default;
        mat2(const mat2& other) noexcept = default;
        mat2& operator=(const mat2& other) noexcept = default;

        mat2(const vec2<T>& row0,
             const vec2<T>& row1) noexcept;

        mat2(T m11, T m12,
             T m21, T m22) noexcept;

        template < typename To >
        mat2<To> cast_to() const noexcept;

        T* data() noexcept;
        const T* data() const noexcept;

        vec2<T>& operator[](std::size_t row) noexcept;
        const vec2<T>& operator[](std::size_t row) const noexcept;

        mat2& operator+=(T v) noexcept;
        mat2& operator*=(T v) noexcept;

        mat2& operator+=(const mat2& other) noexcept;
        mat2& operator*=(const mat2& other) noexcept;
    };
}

namespace e2d
{
    template < typename T >
    const mat2<T>& mat2<T>::zero() noexcept {
        static const mat2<T> zero{
            0, 0,
            0, 0};
        return zero;
    }

    template < typename T >
    const mat2<T>& mat2<T>::identity() noexcept {
        static const mat2<T> identity{
            1, 0,
            0, 1};
        return identity;
    }

    template < typename T >
    mat2<T>::mat2(
        const vec2<T>& row0,
        const vec2<T>& row1) noexcept
    : rows{row0, row1} {}

    template < typename T >
    mat2<T>::mat2(
        T m11, T m12,
        T m21, T m22) noexcept
    : rows{{m11, m12},
           {m21, m22}} {}

    template < typename T >
    template < typename To >
    mat2<To> mat2<T>::cast_to() const noexcept {
        return {
            rows[0].template cast_to<To>(),
            rows[1].template cast_to<To>()};
    }

    template < typename T >
    T* mat2<T>::data() noexcept {
        return rows[0].data();
    }

    template < typename T >
    const T* mat2<T>::data() const noexcept {
        return rows[0].data();
    }

    template < typename T >
    vec2<T>& mat2<T>::operator[](std::size_t row) noexcept {
        E2D_ASSERT(row < 2);
        return rows[row];
    }

    template < typename T >
    const vec2<T>& mat2<T>::operator[](std::size_t row) const noexcept {
        E2D_ASSERT(row < 2);
        return rows[row];
    }

    template < typename T >
    mat2<T>& mat2<T>::operator+=(T v) noexcept {
        return *this = *this + v;
    }

    template < typename T >
    mat2<T>& mat2<T>::operator*=(T v) noexcept {
        return *this = *this * v;
    }

    template < typename T >
    mat2<T>& mat2<T>::operator+=(const mat2& other) noexcept {
        return *this = *this + other;
    }

    template < typename T >
    mat2<T>& mat2<T>::operator*=(const mat2& other) noexcept {
        return *this = *this * other;
    }
}

namespace e2d
{
    //
    // make_mat2
    //

    template < typename T >
    mat2<T> make_mat2(
        const vec2<T>& row0,
        const vec2<T>& row1) noexcept
    {
        return mat2<T>(row0, row1);
    }

    template < typename T >
    mat2<T> make_mat2(
        T m11, T m12,
        T m21, T m22) noexcept
    {
        return mat2<T>(
            m11, m12,
            m21, m22);
    }

    //
    // mat2 (==,!=) mat2
    //

    template < typename T >
    bool operator==(const mat2<T>& l, const mat2<T>& r) noexcept {
        return
            l.rows[0] == r.rows[0] &&
            l.rows[1] == r.rows[1];
    }

    template < typename T >
    bool operator!=(const mat2<T>& l, const mat2<T>& r) noexcept {
        return !(l == r);
    }

    //
    // (-) mat2
    //

    template < typename T >
    mat2<T> operator-(const mat2<T>& m) noexcept {
        return {
            -m.rows[0],
            -m.rows[1]};
    }

    //
    // mat2 (+,*) value
    //

    template < typename T >
    mat2<T> operator+(const mat2<T>& l, T v) noexcept {
        return {
            l.rows[0] + v,
            l.rows[1] + v};
    }

    template < typename T >
    mat2<T> operator*(const mat2<T>& l, T v) noexcept {
        return {
            l.rows[0] * v,
            l.rows[1] * v};
    }

    //
    // value (+,*) mat2
    //

    template < typename T >
    mat2<T> operator+(T v, const mat2<T>& r) noexcept {
        return {
            v + r.rows[0],
            v + r.rows[1]};
    }

    template < typename T >
    mat2<T> operator*(T v, const mat2<T>& r) noexcept {
        return {
            v * r.rows[0],
            v * r.rows[1]};
    }

    //
    // mat2 (+,*) mat2
    //

    template < typename T >
    mat2<T> operator+(const mat2<T>& l, const mat2<T>& r) noexcept {
        return {
            l.rows[0] + r.rows[0],
            l.rows[1] + r.rows[1]};
    }

    template < typename T >
    mat2<T> operator*(const mat2<T>& l, const mat2<T>& r) noexcept {
        const T* const lm = l.data();
        const T* const rm = r.data();
        return {
            lm[0] * rm[0] + lm[1] * rm[2],
            lm[0] * rm[1] + lm[1] * rm[3],

            lm[2] * rm[0] + lm[3] * rm[2],
            lm[2] * rm[1] + lm[3] * rm[3]};
    }

    //
    // vec2 (*) mat2
    //

    template < typename T >
    vec2<T> operator*(const vec2<T>& l, const mat2<T>& r) noexcept {
        const T* const rm = r.data();
        return {
            l.x * rm[0] + l.y * rm[2],
            l.x * rm[1] + l.y * rm[3]};
    }
}

namespace e2d { namespace math
{
    //
    // make_scale_matrix
    //

    template < typename T >
    mat2<T> make_scale_matrix2(T x, T y) noexcept {
        return {
            x, 0,
            0, y};
    }

    template < typename T >
    mat2<T> make_scale_matrix2(const vec4<T>& xy) noexcept {
        return make_scale_matrix2(
            xy.x,
            xy.y);
    }

    template < typename T >
    mat2<T> make_scale_matrix2(const vec3<T>& xy) noexcept {
        return make_scale_matrix2(
            xy.x,
            xy.y);
    }

    template < typename T >
    mat2<T> make_scale_matrix2(const vec2<T>& xy) noexcept {
        return make_scale_matrix2(
            xy.x,
            xy.y);
    }

    //
    // make_rotation_matrix
    //

    template < typename T, typename AngleTag >
    std::enable_if_t<std::is_floating_point<T>::value, mat2<T>>
    make_rotation_matrix2(const unit<T, AngleTag>& angle) noexcept {
        const T cs = math::cos(angle);
        const T sn = math::sin(angle);
        return {
            cs, sn,
            -sn, cs};
    }

    //
    // inversed
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point<T>::value, std::pair<mat2<T>, bool>>
    inversed(
        const mat2<T>& m,
        T precision = math::default_precision<T>()) noexcept
    {
        // https://en.wikipedia.org/wiki/Invertible_matrix
        const T* const mm = m.data();
        const T a = mm[0];
        const T b = mm[1];
        const T c = mm[2];
        const T d = mm[3];
        const T det = a * d - b * c;
        if ( math::is_near_zero(det, precision) ) {
            return std::make_pair(mat2<T>::identity(), false);
        }
        const mat2<T> inv_m(
            d, -b,
            -c, a);
        const T inv_det = T(1) / det;
        return std::make_pair(inv_m * inv_det, true);
    }

    //
    // transposed
    //

    template < typename T >
    mat2<T> transposed(const mat2<T>& m) noexcept {
        const T* const mm = m.data();
        return {
            mm[0], mm[2],
            mm[1], mm[3]};
    }
}}
