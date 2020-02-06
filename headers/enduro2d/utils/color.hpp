/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
        static constexpr color clear() noexcept;
        static constexpr color black() noexcept;
        static constexpr color white() noexcept;
        static constexpr color red() noexcept;
        static constexpr color green() noexcept;
        static constexpr color blue() noexcept;
        static constexpr color yellow() noexcept;
        static constexpr color magenta() noexcept;
        static constexpr color cyan() noexcept;
    public:
        constexpr color() noexcept = default;
        constexpr color(const color& other) noexcept = default;
        constexpr color& operator=(const color& other) noexcept = default;

        constexpr color(f32 r, f32 g, f32 b, f32 a = 1.f) noexcept;

        explicit color(const color32& other) noexcept;
        explicit color(const vec4<f32>& rgba) noexcept;
        explicit color(const vec3<f32>& rgb, f32 a = 1.f) noexcept;

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
    constexpr color color::clear() noexcept {
        return {0, 0, 0, 0};
    }

    constexpr color color::black() noexcept {
        return {0, 0, 0, 1};
    }

    constexpr color color::white() noexcept {
        return {1, 1, 1, 1};
    }

    constexpr color color::red() noexcept {
        return {1, 0, 0, 1};
    }

    constexpr color color::green() noexcept {
        return {0, 1, 0, 1};
    }

    constexpr color color::blue() noexcept {
        return {0, 0, 1, 1};
    }

    constexpr color color::yellow() noexcept {
        return {1, 1, 0, 1};
    }

    constexpr color color::magenta() noexcept {
        return {1, 0, 1, 1};
    }

    constexpr color color::cyan() noexcept {
        return {0, 1, 1, 1};
    }

    constexpr color::color(f32 r, f32 g, f32 b, f32 a) noexcept
    : r(r)
    , g(g)
    , b(b)
    , a(a) {}
}

namespace e2d
{
    vec3<f32> make_vec3(const color& c) noexcept;
    vec4<f32> make_vec4(const color& c) noexcept;

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
}

namespace e2d::colors
{
    u32 pack_color(const color& c) noexcept;
    color unpack_color(u32 argb) noexcept;
}
