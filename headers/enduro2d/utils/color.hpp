/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class color final {
    public:
        f32 r = 1.f;
        f32 g = 1.f;
        f32 b = 1.f;
        f32 a = 1.f;
    public:
        static const color& clear() noexcept;
        static const color& black() noexcept;
        static const color& white() noexcept;
        static const color& red() noexcept;
        static const color& green() noexcept;
        static const color& blue() noexcept;
        static const color& yellow() noexcept;
        static const color& magenta() noexcept;
        static const color& cyan() noexcept;
    public:
        color() noexcept = default;

        color(const color& other) noexcept = default;
        color& operator=(const color& other) noexcept = default;

        color(f32 r, f32 g, f32 b, f32 a = 1.f) noexcept;
        explicit color(const color32& other) noexcept;

        f32* data() noexcept;
        const f32* data() const noexcept;

        f32& operator[](std::size_t index) noexcept;
        f32 operator[](std::size_t index) const noexcept;

        color& operator+=(f32 v) noexcept;
        color& operator-=(f32 v) noexcept;
        color& operator*=(f32 v) noexcept;
        color& operator/=(f32 v) noexcept;

        color& operator+=(const color& other) noexcept;
        color& operator-=(const color& other) noexcept;
        color& operator*=(const color& other) noexcept;
        color& operator/=(const color& other) noexcept;
    };
}

namespace e2d
{
    bool operator<(const color& l, const color& r) noexcept;
    bool operator==(const color& l, const color& r) noexcept;
    bool operator!=(const color& l, const color& r) noexcept;

    color operator+(color l, f32 v) noexcept;
    color operator-(color l, f32 v) noexcept;
    color operator*(color l, f32 v) noexcept;
    color operator/(color l, f32 v) noexcept;

    color operator+(f32 v, const color& r) noexcept;
    color operator-(f32 v, const color& r) noexcept;
    color operator*(f32 v, const color& r) noexcept;
    color operator/(f32 v, const color& r) noexcept;

    color operator+(color l, const color& r) noexcept;
    color operator-(color l, const color& r) noexcept;
    color operator*(color l, const color& r) noexcept;
    color operator/(color l, const color& r) noexcept;
}

namespace e2d::math
{
    bool approximately(const color& l, const color& r) noexcept;
    bool approximately(const color& l, const color& r, f32 precision) noexcept;

    f32 minimum(const color& c) noexcept;
    f32 maximum(const color& c) noexcept;

    color minimized(const color& c, const color& cmin) noexcept;
    color maximized(const color& c, const color& cmax) noexcept;
    color clamped(const color& c, const color& cmin, const color& cmax) noexcept;

    bool contains_nan(const color& c) noexcept;
}

namespace e2d::colors
{
    u32 pack_color(const color& c) noexcept;
    color unpack_color(u32 argb) noexcept;
}
