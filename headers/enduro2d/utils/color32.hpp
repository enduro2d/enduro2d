/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class color32 final {
    public:
        u8 r = 255;
        u8 g = 255;
        u8 b = 255;
        u8 a = 255;
    public:
        static const color32& clear() noexcept;   /// ( 0;   0;   0;   0 )
        static const color32& black() noexcept;   /// ( 0;   0;   0;  255)
        static const color32& white() noexcept;   /// (255; 255; 255; 255)
        static const color32& red() noexcept;     /// (255;  0;   0;  255)
        static const color32& green() noexcept;   /// ( 0;  255;  0;  255)
        static const color32& blue() noexcept;    /// ( 0;   0;  255; 255)
        static const color32& yellow() noexcept;  /// (255; 255;  0;  255)
        static const color32& magenta() noexcept; /// (255;  0;  255; 255)
        static const color32& cyan() noexcept;    /// ( 0;  255; 255; 255)
    public:
        color32() noexcept = default;
        color32(const color32& other) noexcept = default;
        color32& operator=(const color32& other) noexcept = default;

        color32(u8 r, u8 g, u8 b, u8 a = 255) noexcept;
        explicit color32(const color& other) noexcept;

        u8* data() noexcept;
        const u8* data() const noexcept;

        u8& operator[](std::size_t index) noexcept;
        u8  operator[](std::size_t index) const noexcept;

        color32& operator+=(u8 v) noexcept;
        color32& operator-=(u8 v) noexcept;
        color32& operator*=(u8 v) noexcept;
        color32& operator/=(u8 v) noexcept;

        color32& operator+=(const color32& other) noexcept;
        color32& operator-=(const color32& other) noexcept;
        color32& operator*=(const color32& other) noexcept;
        color32& operator/=(const color32& other) noexcept;
    };
}

namespace e2d
{
    bool operator<(const color32& l, const color32& r) noexcept;
    bool operator==(const color32& l, const color32& r) noexcept;
    bool operator!=(const color32& l, const color32& r) noexcept;

    color32 operator+(color32 l, u8 v) noexcept;
    color32 operator-(color32 l, u8 v) noexcept;
    color32 operator*(color32 l, u8 v) noexcept;
    color32 operator/(color32 l, u8 v) noexcept;

    color32 operator+(u8 v, const color32& r) noexcept;
    color32 operator-(u8 v, const color32& r) noexcept;
    color32 operator*(u8 v, const color32& r) noexcept;
    color32 operator/(u8 v, const color32& r) noexcept;

    color32 operator+(color32 l, const color32& r) noexcept;
    color32 operator-(color32 l, const color32& r) noexcept;
    color32 operator*(color32 l, const color32& r) noexcept;
    color32 operator/(color32 l, const color32& r) noexcept;
}

namespace e2d { namespace math
{
    bool approximately(const color32& l, const color32& r) noexcept;
    bool approximately(const color32& l, const color32& r, u8 precision) noexcept;

    u8 minimum(const color32& c) noexcept;
    u8 maximum(const color32& c) noexcept;

    color32 minimized(const color32& c, const color32& cmin) noexcept;
    color32 maximized(const color32& c, const color32& cmax) noexcept;
    color32 clamped(const color32& c, const color32& cmin, const color32& cmax) noexcept;

    bool contains(
        const color32& c,
        u8 value,
        u8 precision = math::default_precision<u8>()) noexcept;
}}

namespace e2d { namespace colors
{
    u32 pack_color32(const color32& c) noexcept;
    color32 unpack_color32(u32 argb) noexcept;
}}
