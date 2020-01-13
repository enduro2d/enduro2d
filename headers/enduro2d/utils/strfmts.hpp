/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "url.hpp"
#include "color.hpp"
#include "color32.hpp"
#include "strings.hpp"

namespace e2d::strings
{
    //
    // vec2
    //

    template < typename T >
    class format_arg<vec2<T>, std::enable_if_t<std::is_integral_v<T>>> {
        vec2<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1)",
                    make_format_arg(value_.x, width_),
                    make_format_arg(value_.y, width_)));
        }
    };

    template < typename T >
    class format_arg<vec2<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        vec2<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1)",
                    make_format_arg(value_.x, width_, precision_),
                    make_format_arg(value_.y, width_, precision_)));
        }
    };

    //
    // vec3
    //

    template < typename T >
    class format_arg<vec3<T>, std::enable_if_t<std::is_integral_v<T>>> {
        vec3<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_.x, width_),
                    make_format_arg(value_.y, width_),
                    make_format_arg(value_.z, width_)));
        }
    };

    template < typename T >
    class format_arg<vec3<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        vec3<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_.x, width_, precision_),
                    make_format_arg(value_.y, width_, precision_),
                    make_format_arg(value_.z, width_, precision_)));
        }
    };

    //
    // vec4
    //

    template < typename T >
    class format_arg<vec4<T>, std::enable_if_t<std::is_integral_v<T>>> {
        vec4<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.x, width_),
                    make_format_arg(value_.y, width_),
                    make_format_arg(value_.z, width_),
                    make_format_arg(value_.w, width_)));
        }
    };

    template < typename T >
    class format_arg<vec4<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        vec4<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.x, width_, precision_),
                    make_format_arg(value_.y, width_, precision_),
                    make_format_arg(value_.z, width_, precision_),
                    make_format_arg(value_.w, width_, precision_)));
        }
    };

    //
    // ray2
    //

    template < typename T >
    class format_arg<ray2<T>, std::enable_if_t<std::is_integral_v<T>>> {
        ray2<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.origin.x, width_),
                    make_format_arg(value_.origin.y, width_),
                    make_format_arg(value_.direction.x, width_),
                    make_format_arg(value_.direction.y, width_)));
        }
    };

    template < typename T >
    class format_arg<ray2<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        ray2<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.origin.x, width_, precision_),
                    make_format_arg(value_.origin.y, width_, precision_),
                    make_format_arg(value_.direction.x, width_, precision_),
                    make_format_arg(value_.direction.y, width_, precision_)));
        }
    };

    //
    // ray3
    //

    template < typename T >
    class format_arg<ray3<T>, std::enable_if_t<std::is_integral_v<T>>> {
        ray3<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3,%4,%5)",
                    make_format_arg(value_.origin.x, width_),
                    make_format_arg(value_.origin.y, width_),
                    make_format_arg(value_.origin.z, width_),
                    make_format_arg(value_.direction.x, width_),
                    make_format_arg(value_.direction.y, width_),
                    make_format_arg(value_.direction.z, width_)));
        }
    };

    template < typename T >
    class format_arg<ray3<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        ray3<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3,%4,%5)",
                    make_format_arg(value_.origin.x, width_, precision_),
                    make_format_arg(value_.origin.y, width_, precision_),
                    make_format_arg(value_.origin.z, width_, precision_),
                    make_format_arg(value_.direction.x, width_, precision_),
                    make_format_arg(value_.direction.y, width_, precision_),
                    make_format_arg(value_.direction.z, width_, precision_)));
        }
    };

    //
    // rect
    //

    template < typename T >
    class format_arg<rect<T>, std::enable_if_t<std::is_integral_v<T>>> {
        rect<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.position.x, width_),
                    make_format_arg(value_.position.y, width_),
                    make_format_arg(value_.size.x, width_),
                    make_format_arg(value_.size.y, width_)));
        }
    };

    template < typename T >
    class format_arg<rect<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        rect<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.position.x, width_, precision_),
                    make_format_arg(value_.position.y, width_, precision_),
                    make_format_arg(value_.size.x, width_, precision_),
                    make_format_arg(value_.size.y, width_, precision_)));
        }
    };

    //
    // aabb
    //

    template < typename T >
    class format_arg<aabb<T>, std::enable_if_t<std::is_integral_v<T>>> {
        aabb<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3,%4,%5)",
                    make_format_arg(value_.position.x, width_),
                    make_format_arg(value_.position.y, width_),
                    make_format_arg(value_.position.z, width_),
                    make_format_arg(value_.size.x, width_),
                    make_format_arg(value_.size.y, width_),
                    make_format_arg(value_.size.z, width_)));
        }
    };

    template < typename T >
    class format_arg<aabb<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        aabb<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3,%4,%5)",
                    make_format_arg(value_.position.x, width_, precision_),
                    make_format_arg(value_.position.y, width_, precision_),
                    make_format_arg(value_.position.z, width_, precision_),
                    make_format_arg(value_.size.x, width_, precision_),
                    make_format_arg(value_.size.y, width_, precision_),
                    make_format_arg(value_.size.z, width_, precision_)));
        }
    };

    //
    // mat2
    //

    template < typename T >
    class format_arg<mat2<T>, std::enable_if_t<std::is_integral_v<T>>> {
        mat2<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1)",
                    make_format_arg(value_[0], width_),
                    make_format_arg(value_[1], width_)));
        }
    };

    template < typename T >
    class format_arg<mat2<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        mat2<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1)",
                    make_format_arg(value_[0], width_, precision_),
                    make_format_arg(value_[1], width_, precision_)));
        }
    };

    //
    // mat3
    //

    template < typename T >
    class format_arg<mat3<T>, std::enable_if_t<std::is_integral_v<T>>> {
        mat3<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_[0], width_),
                    make_format_arg(value_[1], width_),
                    make_format_arg(value_[2], width_)));
        }
    };

    template < typename T >
    class format_arg<mat3<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        mat3<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_[0], width_, precision_),
                    make_format_arg(value_[1], width_, precision_),
                    make_format_arg(value_[2], width_, precision_)));
        }
    };

    //
    // mat4
    //

    template < typename T >
    class format_arg<mat4<T>, std::enable_if_t<std::is_integral_v<T>>> {
        mat4<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_[0], width_),
                    make_format_arg(value_[1], width_),
                    make_format_arg(value_[2], width_),
                    make_format_arg(value_[3], width_)));
        }
    };

    template < typename T >
    class format_arg<mat4<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        mat4<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_[0], width_, precision_),
                    make_format_arg(value_[1], width_, precision_),
                    make_format_arg(value_[2], width_, precision_),
                    make_format_arg(value_[3], width_, precision_)));
        }
    };

    //
    // rad
    //

    template < typename T >
    class format_arg<rad<T>, std::enable_if_t<std::is_integral_v<T>>> {
        rad<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0rad",
                    make_format_arg(value_.value, width_)));
        }
    };

    template < typename T >
    class format_arg<rad<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        rad<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0rad",
                    make_format_arg(value_.value, width_, precision_)));
        }
    };

    //
    // deg
    //

    template < typename T >
    class format_arg<deg<T>, std::enable_if_t<std::is_integral_v<T>>> {
        deg<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0deg",
                    make_format_arg(value_.value, width_)));
        }
    };

    template < typename T >
    class format_arg<deg<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        deg<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0deg",
                    make_format_arg(value_.value, width_, precision_)));
        }
    };

    //
    // trs2
    //

    template < typename T >
    class format_arg<trs2<T>, std::enable_if_t<std::is_integral_v<T>>> {
        trs2<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_.translation, width_),
                    make_format_arg(value_.rotation, width_),
                    make_format_arg(value_.scale, width_)));
        }
    };

    template < typename T >
    class format_arg<trs2<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        trs2<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_.translation, width_, precision_),
                    make_format_arg(value_.rotation, width_, precision_),
                    make_format_arg(value_.scale, width_, precision_)));
        }
    };

    //
    // trs3
    //

    template < typename T >
    class format_arg<trs3<T>, std::enable_if_t<std::is_integral_v<T>>> {
        trs3<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_.translation, width_),
                    make_format_arg(value_.rotation, width_),
                    make_format_arg(value_.scale, width_)));
        }
    };

    template < typename T >
    class format_arg<trs3<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        trs3<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2)",
                    make_format_arg(value_.translation, width_, precision_),
                    make_format_arg(value_.rotation, width_, precision_),
                    make_format_arg(value_.scale, width_, precision_)));
        }
    };

    //
    // url
    //

    template <>
    class format_arg<url> {
        url value_;
    public:
        template < typename U >
        explicit format_arg(U&& value)
            noexcept(noexcept(std::is_nothrow_constructible_v<url, U>))
        : value_(std::forward<U>(value)) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0://%1",
                    value_.scheme(), value_.path()));
        }
    };

    //
    // str
    //

    template <>
    class format_arg<str> {
        str value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0)
            noexcept(noexcept(std::is_nothrow_constructible_v<str, U>))
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0",
                    make_format_arg(value_.c_str(), width_)));
        }
    };

    template <>
    class format_arg<str_view> : public format_arg<str> {
    public:
        explicit format_arg(str_view sv, u8 width = 0)
        : format_arg<str>(sv, width) {}
    };

    //
    // wstr
    //

    template <>
    class format_arg<wstr> {
        wstr value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0)
            noexcept(noexcept(std::is_nothrow_constructible_v<wstr, U>))
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0",
                    make_format_arg(make_utf8(value_), width_)));
        }
    };

    template <>
    class format_arg<wstr_view> : public format_arg<wstr> {
    public:
        explicit format_arg(wstr_view sv, u8 width = 0)
        : format_arg<wstr>(sv, width) {}
    };

    //
    // str16
    //

    template <>
    class format_arg<str16> {
        str16 value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0)
            noexcept(noexcept(std::is_nothrow_constructible_v<str16, U>))
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0",
                    make_format_arg(make_utf8(value_), width_)));
        }
    };

    template <>
    class format_arg<str16_view> : public format_arg<str16> {
    public:
        explicit format_arg(str16_view sv, u8 width = 0)
        : format_arg<str16>(sv, width) {}
    };

    //
    // str32
    //

    template <>
    class format_arg<str32> {
        str32 value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0)
            noexcept(noexcept(std::is_nothrow_constructible_v<str32, U>))
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0",
                    make_format_arg(make_utf8(value_), width_)));
        }
    };

    template <>
    class format_arg<str32_view> : public format_arg<str32> {
    public:
        explicit format_arg(str32_view sv, u8 width = 0)
        : format_arg<str32>(sv, width) {}
    };

    //
    // color
    //

    template <>
    class format_arg<color> {
        color value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.r, width_, precision_),
                    make_format_arg(value_.g, width_, precision_),
                    make_format_arg(value_.b, width_, precision_),
                    make_format_arg(value_.a, width_, precision_)));
        }
    };

    //
    // color32
    //

    template <>
    class format_arg<color32> {
        color32 value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1,%2,%3)",
                    make_format_arg(value_.r, width_),
                    make_format_arg(value_.g, width_),
                    make_format_arg(value_.b, width_),
                    make_format_arg(value_.a, width_)));
        }
    };

    //
    // seconds
    //

    template < typename T >
    class format_arg<seconds<T>, std::enable_if_t<std::is_integral_v<T>>> {
        seconds<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0s",
                    make_format_arg(value_.value, width_)));
        }
    };

    template < typename T >
    class format_arg<seconds<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        seconds<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0s",
                    make_format_arg(value_.value, width_, precision_)));
        }
    };

    //
    // milliseconds
    //

    template < typename T >
    class format_arg<milliseconds<T>, std::enable_if_t<std::is_integral_v<T>>> {
        milliseconds<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0ms",
                    make_format_arg(value_.value, width_)));
        }
    };

    template < typename T >
    class format_arg<milliseconds<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        milliseconds<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0ms",
                    make_format_arg(value_.value, width_, precision_)));
        }
    };

    //
    // microseconds
    //

    template < typename T >
    class format_arg<microseconds<T>, std::enable_if_t<std::is_integral_v<T>>> {
        microseconds<T> value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0us",
                    make_format_arg(value_.value, width_)));
        }
    };

    template < typename T >
    class format_arg<microseconds<T>, std::enable_if_t<std::is_floating_point_v<T>>> {
        microseconds<T> value_;
        u8 width_;
        u8 precision_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0, u8 precision = 6) noexcept
        : value_(std::forward<U>(value)), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0us",
                    make_format_arg(value_.value, width_, precision_)));
        }
    };

    //
    // enum
    //

    template < typename T >
    class format_arg<T, std::enable_if_t<std::is_enum_v<T>>> {
        T value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0",
                    make_format_arg(enum_hpp::to_string_or_throw(value_), width_)));
        }
    };
}
