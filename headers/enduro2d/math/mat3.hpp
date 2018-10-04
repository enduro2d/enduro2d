/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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
    class mat3 final {
        static_assert(
            std::is_arithmetic<T>::value,
            "type of 'mat3' must be arithmetic");
    public:
        using self_type = mat3;
        using value_type = T;
    public:
        vec3<T> rows[3] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}};
    public:
        static const mat3& zero() noexcept;
        static const mat3& identity() noexcept;
    public:
        mat3() noexcept = default;
        mat3(const mat3& other) noexcept = default;
        mat3& operator=(const mat3& other) noexcept = default;

        mat3(const vec3<T>& row0,
             const vec3<T>& row1,
             const vec3<T>& row2) noexcept;

        mat3(T m11, T m12, T m13,
             T m21, T m22, T m23,
             T m31, T m32, T m33) noexcept;

        template < typename To >
        mat3<To> cast_to() const noexcept;
        const T* data() const noexcept;

        vec3<T>& operator[](std::size_t row) noexcept;
        const vec3<T>& operator[](std::size_t row) const noexcept;

        mat3& operator+=(T v) noexcept;
        mat3& operator*=(T v) noexcept;

        mat3& operator+=(const mat3& other) noexcept;
        mat3& operator*=(const mat3& other) noexcept;
    };
}

namespace e2d
{
    template < typename T >
    const mat3<T>& mat3<T>::zero() noexcept {
        static const mat3<T> zero{
            0, 0, 0,
            0, 0, 0,
            0, 0, 0};
        return zero;
    }

    template < typename T >
    const mat3<T>& mat3<T>::identity() noexcept {
        static const mat3<T> identity{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1};
        return identity;
    }

    template < typename T >
    mat3<T>::mat3(
        const vec3<T>& row0,
        const vec3<T>& row1,
        const vec3<T>& row2) noexcept
    : rows{row0, row1, row2} {}

    template < typename T >
    mat3<T>::mat3(
        T m11, T m12, T m13,
        T m21, T m22, T m23,
        T m31, T m32, T m33) noexcept
    : rows{{m11, m12, m13},
           {m21, m22, m23},
           {m31, m32, m33}} {}

    template < typename T >
    template < typename To >
    mat3<To> mat3<T>::cast_to() const noexcept {
        return {
            rows[0].template cast_to<To>(),
            rows[1].template cast_to<To>(),
            rows[2].template cast_to<To>()};
    }

    template < typename T >
    const T* mat3<T>::data() const noexcept {
        return rows[0].data();
    }

    template < typename T >
    vec3<T>& mat3<T>::operator[](std::size_t row) noexcept {
        E2D_ASSERT(row < 3);
        return rows[row];
    }

    template < typename T >
    const vec3<T>& mat3<T>::operator[](std::size_t row) const noexcept {
        E2D_ASSERT(row < 3);
        return rows[row];
    }

    template < typename T >
    mat3<T>& mat3<T>::operator+=(T v) noexcept {
        rows[0] += v;
        rows[1] += v;
        rows[2] += v;
        return *this;
    }

    template < typename T >
    mat3<T>& mat3<T>::operator*=(T v) noexcept {
        rows[0] *= v;
        rows[1] *= v;
        rows[2] *= v;
        return *this;
    }

    template < typename T >
    mat3<T>& mat3<T>::operator+=(const mat3& other) noexcept {
        rows[0] += other.rows[0];
        rows[1] += other.rows[1];
        rows[2] += other.rows[2];
        return *this;
    }

    template < typename T >
    mat3<T>& mat3<T>::operator*=(const mat3& other) noexcept {
        rows[0] *= other.rows[0];
        rows[1] *= other.rows[1];
        rows[2] *= other.rows[2];
        return *this;
    }
}

namespace e2d
{
    //
    // make_mat3
    //

    template < typename T >
    mat3<T> make_mat3(
        const vec3<T>& row0,
        const vec3<T>& row1,
        const vec3<T>& row2) noexcept
    {
        return mat3<T>(row0, row1, row2);
    }

    template < typename T >
    mat3<T> make_mat3(
        T m11, T m12, T m13,
        T m21, T m22, T m23,
        T m31, T m32, T m33) noexcept
    {
        return mat3<T>(
            m11, m12, m13,
            m21, m22, m23,
            m31, m32, m33);
    }

    //
    // mat3 (==,!=) mat3
    //

    template < typename T >
    bool operator==(const mat3<T>& l, const mat3<T>& r) noexcept {
        return
            l.rows[0] == r.rows[0] &&
            l.rows[1] == r.rows[1] &&
            l.rows[2] == r.rows[2];
    }

    template < typename T >
    bool operator!=(const mat3<T>& l, const mat3<T>& r) noexcept {
        return !(l == r);
    }

    //
    // (-) mat3
    //

    template < typename T >
    mat3<T> operator-(const mat3<T>& m) noexcept {
        return {
            -m.rows[0],
            -m.rows[1],
            -m.rows[2]};
    }

    //
    // mat3 (+,*) value
    //

    template < typename T >
    mat3<T> operator+(const mat3<T>& l, T v) noexcept {
        return {
            l.rows[0] + v,
            l.rows[1] + v,
            l.rows[2] + v};
    }

    template < typename T >
    mat3<T> operator*(const mat3<T>& l, T v) noexcept {
        return {
            l.rows[0] * v,
            l.rows[1] * v,
            l.rows[2] * v};
    }

    //
    // value (+,*) mat3
    //

    template < typename T >
    mat3<T> operator+(T v, const mat3<T>& r) noexcept {
        return {
            v + r.rows[0],
            v + r.rows[1],
            v + r.rows[2]};
    }

    template < typename T >
    mat3<T> operator*(T v, const mat3<T>& r) noexcept {
        return {
            v * r.rows[0],
            v * r.rows[1],
            v * r.rows[2]};
    }

    //
    // mat3 (+,*) mat3
    //

    template < typename T >
    mat3<T> operator+(const mat3<T>& l, const mat3<T>& r) noexcept {
        return {
            l.rows[0] + r.rows[0],
            l.rows[1] + r.rows[1],
            l.rows[2] + r.rows[2]};
    }

    template < typename T >
    mat3<T> operator*(const mat3<T>& l, const mat3<T>& r) noexcept {
        const T* const lm = l.data();
        const T* const rm = r.data();
        return {
            lm[0] * rm[0] + lm[1] * rm[3] + lm[2] * rm[6],
            lm[0] * rm[1] + lm[1] * rm[4] + lm[2] * rm[7],
            lm[0] * rm[2] + lm[1] * rm[5] + lm[2] * rm[8],

            lm[3] * rm[0] + lm[4] * rm[3] + lm[5] * rm[6],
            lm[3] * rm[1] + lm[4] * rm[4] + lm[5] * rm[7],
            lm[3] * rm[2] + lm[4] * rm[5] + lm[5] * rm[8],

            lm[6] * rm[0] + lm[7] * rm[3] + lm[8] * rm[6],
            lm[6] * rm[1] + lm[7] * rm[4] + lm[8] * rm[7],
            lm[6] * rm[2] + lm[7] * rm[5] + lm[8] * rm[8]};
    }

    //
    // vec3 (*) mat3
    //

    template < typename T >
    vec3<T> operator*(const vec3<T>& l, const mat3<T>& r) noexcept {
        const T* const rm = r.data();
        return {
            l.x * rm[0] + l.y * rm[3] + l.z * rm[6],
            l.x * rm[1] + l.y * rm[4] + l.z * rm[7],
            l.x * rm[2] + l.y * rm[5] + l.z * rm[8]};
    }
}

namespace e2d { namespace math
{
    //
    // make_scale_matrix
    //

    template < typename T >
    mat3<T> make_scale_matrix3(T x, T y, T z = T(1)) noexcept {
        return {
            x, 0, 0,
            0, y, 0,
            0, 0, z};
    }

    template < typename T >
    mat3<T> make_scale_matrix3(const vec4<T>& xyz) noexcept {
        return make_scale_matrix3(
            xyz.x,
            xyz.y,
            xyz.z);
    }

    template < typename T >
    mat3<T> make_scale_matrix3(const vec3<T>& xyz) noexcept {
        return make_scale_matrix3(
            xyz.x,
            xyz.y,
            xyz.z);
    }

    template < typename T >
    mat3<T> make_scale_matrix3(const vec2<T>& xy, T z = T(1)) noexcept {
        return make_scale_matrix3(
            xy.x,
            xy.y,
            z);
    }

    //
    // make_rotation_matrix
    //

    template < typename T, typename AngleTag >
    std::enable_if_t<std::is_floating_point<T>::value, mat3<T>>
    make_rotation_matrix3(
        const unit<T, AngleTag>& angle,
        T axis_x,
        T axis_y,
        T axis_z) noexcept
    {
        const T x   = axis_x;
        const T y   = axis_y;
        const T z   = axis_z;
        const T px  = x * x;
        const T py  = y * y;
        const T pz  = z * z;
        const T cs  = math::cos(angle);
        const T sn  = math::sin(angle);
        const T ics = T(1) - cs;
        const T xym = x * y * ics;
        const T xzm = x * z * ics;
        const T yzm = y * z * ics;
        const T xsn = x * sn;
        const T ysn = y * sn;
        const T zsn = z * sn;
        return {
            px * ics + cs, xym - zsn,     xzm + ysn,
            xym + zsn,     py * ics + cs, yzm - xsn,
            xzm - ysn,     yzm + xsn,     pz * ics + cs};
    }

    template < typename T, typename AngleTag >
    mat3<T> make_rotation_matrix3(
        const unit<T, AngleTag>& angle,
        const vec4<T>& axis_xyz) noexcept
    {
        return make_rotation_matrix3(
            angle,
            axis_xyz.x,
            axis_xyz.y,
            axis_xyz.z);
    }

    template < typename T, typename AngleTag >
    mat3<T> make_rotation_matrix3(
        const unit<T, AngleTag>& angle,
        const vec3<T>& axis_xyz) noexcept
    {
        return make_rotation_matrix3(
            angle,
            axis_xyz.x,
            axis_xyz.y,
            axis_xyz.z);
    }

    template < typename T, typename AngleTag >
    mat3<T> make_rotation_matrix3(
        const unit<T, AngleTag>& angle,
        const vec2<T>& axis_xy,
        T axis_z) noexcept
    {
        return make_rotation_matrix3(
            angle,
            axis_xy.x,
            axis_xy.y,
            axis_z);
    }

    //
    // inversed
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point<T>::value, std::pair<mat3<T>, bool>>
    inversed(
        const mat3<T>& m,
        T precision = math::default_precision<T>()) noexcept
    {
        // https://en.wikipedia.org/wiki/Invertible_matrix
        const T* const mm = m.data();
        const T a = mm[0];
        const T b = mm[1];
        const T c = mm[2];
        const T d = mm[3];
        const T e = mm[4];
        const T f = mm[5];
        const T g = mm[6];
        const T h = mm[7];
        const T i = mm[8];
        const T A = e * i - f * h;
        const T B = c * h - b * i;
        const T C = b * f - c * e;
        const T det = a * A + d * B + g * C;
        if ( math::is_near_zero(det, precision) ) {
            return std::make_pair(mat3<T>::identity(), false);
        }
        const T D = f * g - d * i;
        const T E = a * i - c * g;
        const T F = c * d - a * f;
        const T G = d * h - e * g;
        const T H = b * g - a * h;
        const T I = a * e - b * d;
        const mat3<T> inv_m(
            A, B, C,
            D, E, F,
            G, H, I);
        const T inv_det = T(1) / det;
        return std::make_pair(inv_m * inv_det, true);
    }

    //
    // transposed
    //

    template < typename T >
    mat3<T> transposed(const mat3<T>& m) noexcept {
        const T* const mm = m.data();
        return {
            mm[0], mm[3], mm[6],
            mm[1], mm[4], mm[7],
            mm[2], mm[5], mm[8]};
    }
}}
