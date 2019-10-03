/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "trig.hpp"
#include "unit.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace e2d
{
    template < typename T >
    class quat final {
        static_assert(
            std::is_arithmetic_v<T>,
            "type of 'quat' must be arithmetic");
    public:
        using self_type = quat;
        using value_type = T;
    public:
        T x = 0;
        T y = 0;
        T z = 0;
        T w = 1;
    public:
        static constexpr quat zero() noexcept;
        static constexpr quat identity() noexcept;
    public:
        constexpr quat() noexcept = default;
        constexpr quat(const quat& other) noexcept = default;
        constexpr quat& operator=(const quat& other) noexcept = default;

        constexpr quat(T x, T y, T z, T w) noexcept;
        constexpr explicit quat(const vec4<T>& other) noexcept;

        template < typename To >
        quat<To> cast_to() const noexcept;

        T* data() noexcept;
        const T* data() const noexcept;

        T& operator[](std::size_t index) noexcept;
        T operator[](std::size_t index) const noexcept;

        quat& operator+=(T v) noexcept;
        quat& operator*=(T v) noexcept;

        quat& operator+=(const quat& other) noexcept;
        quat& operator*=(const quat& other) noexcept;
    };
}

namespace e2d
{
    template < typename T >
    constexpr quat<T> quat<T>::zero() noexcept {
        return {0, 0, 0, 0};
    }

    template < typename T >
    constexpr quat<T> quat<T>::identity() noexcept {
        return {0, 0, 0, 1};
    }

    template < typename T >
    constexpr quat<T>::quat(T nx, T ny, T nz, T nw) noexcept
    : x(nx)
    , y(ny)
    , z(nz)
    , w(nw) {}

    template < typename T >
    constexpr quat<T>::quat(const vec4<T>& other) noexcept
    : x(other.x)
    , y(other.y)
    , z(other.z)
    , w(other.w) {}

    template < typename T >
    template < typename To >
    quat<To> quat<T>::cast_to() const noexcept {
        return {
            math::numeric_cast<To>(x),
            math::numeric_cast<To>(y),
            math::numeric_cast<To>(z),
            math::numeric_cast<To>(w)};
    }

    template < typename T >
    T* quat<T>::data() noexcept {
        return &x;
    }

    template < typename T >
    const T* quat<T>::data() const noexcept {
        return &x;
    }

    template < typename T >
    T& quat<T>::operator[](std::size_t index) noexcept {
        E2D_ASSERT(index < 4);
        return data()[index];
    }

    template < typename T >
    T quat<T>::operator[](std::size_t index) const noexcept {
        E2D_ASSERT(index < 4);
        return data()[index];
    }

    template < typename T >
    quat<T>& quat<T>::operator+=(T v) noexcept {
        return *this = *this + v;
    }

    template < typename T >
    quat<T>& quat<T>::operator*=(T v) noexcept {
        return *this = *this * v;
    }

    template < typename T >
    quat<T>& quat<T>::operator+=(const quat& other) noexcept {
        return *this = *this + other;
    }

    template < typename T >
    quat<T>& quat<T>::operator*=(const quat& other) noexcept {
        return *this = *this * other;
    }
}

namespace e2d
{
    //
    // make_quat
    //

    template < typename T >
    constexpr quat<T> make_quat(T x, T y, T z, T w) noexcept {
        return quat<T>(x, y, z, w);
    }

    template < typename T >
    constexpr quat<T> make_quat(const vec4<T>& other) noexcept {
        return quat<T>(other);
    }

    //
    // quat (==,!=) quat
    //

    template < typename T >
    bool operator==(const quat<T>& l, const quat<T>& r) noexcept {
        return
            math::approximately(l.x, r.x) &&
            math::approximately(l.y, r.y) &&
            math::approximately(l.z, r.z) &&
            math::approximately(l.w, r.w);
    }

    template < typename T >
    bool operator!=(const quat<T>& l, const quat<T>& r) noexcept {
        return !(l == r);
    }

    //
    // quat (<,>,<=,>=) quat
    //

    template < typename T >
    bool operator<(const quat<T>& l, const quat<T>& r) noexcept {
        return
            (l.x < r.x) ||
            (l.x == r.x && l.y < r.y) ||
            (l.x == r.x && l.y == r.y && l.z < r.z) ||
            (l.x == r.x && l.y == r.y && l.z == r.z && l.w < r.w);
    }

    template < typename T >
    bool operator>(const quat<T>& l, const quat<T>& r) noexcept {
        return r < l;
    }

    template < typename T >
    bool operator<=(const quat<T>& l, const quat<T>& r) noexcept {
        return !(r < l);
    }

    template < typename T >
    bool operator>=(const quat<T>& l, const quat<T>& r) noexcept {
        return !(l < r);
    }

    //
    // quat (+,*) value
    //

    template < typename T >
    quat<T> operator+(const quat<T>& l, T v) noexcept {
        return {
            l.x + v,
            l.y + v,
            l.z + v,
            l.w + v};
    }

    template < typename T >
    quat<T> operator*(const quat<T>& l, T v) noexcept {
        return {
            l.x * v,
            l.y * v,
            l.z * v,
            l.w * v};
    }

    //
    // value (+,*) quat
    //

    template < typename T >
    quat<T> operator+(T v, const quat<T>& r) noexcept {
        return {
            v + r.x,
            v + r.y,
            v + r.z,
            v + r.w};
    }

    template < typename T >
    quat<T> operator*(T v, const quat<T>& r) noexcept {
        return {
            v * r.x,
            v * r.y,
            v * r.z,
            v * r.w};
    }

    //
    // quat (+,*) quat
    //

    template < typename T >
    quat<T> operator+(const quat<T>& l, const quat<T>& r) noexcept {
        return {
            l.x + r.x,
            l.y + r.y,
            l.z + r.z,
            l.w + r.w};
    }

    template < typename T >
    quat<T> operator*(const quat<T>& l, const quat<T>& r) noexcept {
        return {
            (l.w * r.x) + (l.x * r.w) + (l.y * r.z) - (l.z * r.y),
            (l.w * r.y) + (l.y * r.w) + (l.z * r.x) - (l.x * r.z),
            (l.w * r.z) + (l.z * r.w) + (l.x * r.y) - (l.y * r.x),
            (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z)};
    }

    //
    // vec3 (*) quat
    //

    template < typename T >
    vec3<T> operator*(const vec3<T>& l, const quat<T>& r) noexcept {
        const vec3<T> qvec{r.x, r.y, r.z};
        auto uv = math::cross(qvec, l);
        auto uuv = math::cross(qvec, uv);
        uv *= r.w * T(2);
        uuv *= T(2);
        return l + uv + uuv;
    }
}

namespace e2d::math
{
    //
    // make_quat_from_axis_angle
    //

    template < typename T, typename AngleTag >
    std::enable_if_t<std::is_floating_point_v<T>, quat<T>>
    make_quat_from_axis_angle(
        const unit<T, AngleTag>& angle,
        const vec3<T>& axis_xyz) noexcept
    {
        const unit<T, AngleTag> ha = T(0.5) * angle;
        const T sa = math::sin(ha);
        const T ca = math::cos(ha);
        return {
            sa * axis_xyz.x,
            sa * axis_xyz.y,
            sa * axis_xyz.z,
            ca};
    }

    //
    // make_quat_from_euler_angles
    //

    template < typename T
             , typename AngleTagR
             , typename AngleTagP
             , typename AngleTagY >
    std::enable_if_t<std::is_floating_point_v<T>, quat<T>>
    make_quat_from_euler_angles(
        const unit<T, AngleTagR>& roll,
        const unit<T, AngleTagP>& pitch,
        const unit<T, AngleTagY>& yaw) noexcept
    {
        const unit<T, AngleTagR> hr = T(0.5) * roll;
        const T sr = math::sin(hr);
        const T cr = math::cos(hr);

        const unit<T, AngleTagP> hp = T(0.5) * pitch;
        const T sp = math::sin(hp);
        const T cp = math::cos(hp);

        const unit<T, AngleTagY> hy = T(0.5) * yaw;
        const T sy = math::sin(hy);
        const T cy = math::cos(hy);

        const T cpcy = cp * cy;
        const T spcy = sp * cy;
        const T cpsy = cp * sy;
        const T spsy = sp * sy;

        return {
            sr * cpcy - cr * spsy,
            cr * spcy + sr * cpsy,
            cr * cpsy - sr * spcy,
            cr * cpcy + sr * spsy};
    }

    //
    // approximately
    //

    template < typename T >
    bool approximately(
        const quat<T>& l,
        const quat<T>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return
            math::approximately(l.x, r.x, precision) &&
            math::approximately(l.y, r.y, precision) &&
            math::approximately(l.z, r.z, precision) &&
            math::approximately(l.w, r.w, precision);
    }

    //
    // dot
    //

    template < typename T >
    T dot(const quat<T>& l, const quat<T>& r) noexcept {
        return
            l.x * r.x +
            l.y * r.y +
            l.z * r.z +
            l.w * r.w;
    }

    template < typename T >
    T abs_dot(const quat<T>& l, const quat<T>& r) noexcept {
        return
            math::abs(l.x * r.x) +
            math::abs(l.y * r.y) +
            math::abs(l.z * r.z) +
            math::abs(l.w * r.w);
    }

    //
    // length
    //

    template < typename T >
    T length_squared(const quat<T>& v) noexcept {
        return dot(v, v);
    }

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    length(const quat<T>& v) noexcept {
        return math::sqrt(length_squared(v));
    }

    //
    // normalized
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, quat<T>>
    normalized(
        const quat<T>& v,
        T precision = math::default_precision<T>()) noexcept
    {
        const T l = length(v);
        if ( !math::is_near_zero(l, precision) ) {
            const T inv_l = T(1) / l;
            return v * inv_l;
        }
        return quat<T>::identity();
    }

    //
    // lerp
    //

    template < typename T >
    quat<T> lerp(const quat<T>& l, const quat<T>& r, T v) noexcept {
        return {
            math::lerp(l.x, r.x, v),
            math::lerp(l.y, r.y, v),
            math::lerp(l.z, r.z, v),
            math::lerp(l.w, r.w, v)};
    }

    //
    // inversed
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, quat<T>>
    inversed(
        const quat<T>& q,
        T precision = math::default_precision<T>()) noexcept
    {
        const T l = length(q);
        if ( !math::is_near_zero(l, precision) ) {
            const T inv_l = T(1) / l;
            return {
                -q.x * inv_l,
                -q.y * inv_l,
                -q.z * inv_l,
                 q.w * inv_l};
        }
        return quat<T>::identity();
    }

    //
    // conjugated
    //

    template < typename T >
    quat<T> conjugated(const quat<T>& q) noexcept {
        return {
            -q.x,
            -q.y,
            -q.z,
            q.w};
    }

    //
    // contains_nan
    //

    template < typename T >
    bool contains_nan(const quat<T>& v) noexcept {
        return
            !math::is_finite(v.x) ||
            !math::is_finite(v.y) ||
            !math::is_finite(v.z) ||
            !math::is_finite(v.w);
    }
}
