/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "color.hpp"
#include "color32.hpp"
#include "strings.hpp"

namespace e2d { namespace strings
{
    //
    // vec2
    //

    template < typename T >
    class format_arg<vec2<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<vec2<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<vec3<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<vec3<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<vec4<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<vec4<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    // mat2
    //

    template < typename T >
    class format_arg<mat2<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<mat2<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<mat3<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<mat3<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<mat4<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<mat4<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<rad<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<rad<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<deg<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<deg<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    // str
    //

    template <>
    class format_arg<str> {
        str value_;
    public:
        template < typename U >
        explicit format_arg(U&& value)
            noexcept(noexcept(std::is_nothrow_constructible<str, U>::value))
        : value_(std::forward<U>(value)) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0", value_.c_str()));
        }
    };

    template <>
    class format_arg<str_view> : public format_arg<str> {
    public:
        explicit format_arg(str_view sv)
        : format_arg<str>(sv) {}
    };

    //
    // wstr
    //

    template <>
    class format_arg<wstr> {
        wstr value_;
    public:
        template < typename U >
        explicit format_arg(U&& value)
            noexcept(noexcept(std::is_nothrow_constructible<wstr, U>::value))
        : value_(std::forward<U>(value)) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0", make_utf8(value_.c_str())));
        }
    };

    template <>
    class format_arg<wstr_view> : public format_arg<wstr> {
    public:
        explicit format_arg(wstr_view sv)
        : format_arg<wstr>(sv) {}
    };

    //
    // str16
    //

    template <>
    class format_arg<str16> {
        str16 value_;
    public:
        template < typename U >
        explicit format_arg(U&& value)
            noexcept(noexcept(std::is_nothrow_constructible<str16, U>::value))
        : value_(std::forward<U>(value)) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0", make_utf8(value_.c_str())));
        }
    };

    template <>
    class format_arg<str16_view> : public format_arg<str16> {
    public:
        explicit format_arg(str16_view sv)
        : format_arg<str16>(sv) {}
    };

    //
    // str32
    //

    template <>
    class format_arg<str32> {
        str32 value_;
    public:
        template < typename U >
        explicit format_arg(U&& value)
            noexcept(noexcept(std::is_nothrow_constructible<str32, U>::value))
        : value_(std::forward<U>(value)) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "%0", make_utf8(value_.c_str())));
        }
    };

    template <>
    class format_arg<str32_view> : public format_arg<str32> {
    public:
        explicit format_arg(str32_view sv)
        : format_arg<str32>(sv) {}
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
    class format_arg<seconds<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<seconds<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<milliseconds<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<milliseconds<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
    class format_arg<microseconds<T>, std::enable_if_t<std::is_integral<T>::value>> {
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
    class format_arg<microseconds<T>, std::enable_if_t<std::is_floating_point<T>::value>> {
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
}}
