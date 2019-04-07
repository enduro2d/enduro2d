/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/color.hpp>
#include <enduro2d/utils/color32.hpp>

namespace e2d
{
    const color& color::clear() noexcept {
        static const color c(0, 0, 0, 0);
        return c;
    }

    const color& color::black() noexcept {
        static const color c(0, 0, 0, 1);
        return c;
    }

    const color& color::white() noexcept {
        static const color c(1, 1, 1, 1);
        return c;
    }

    const color& color::red() noexcept {
        static const color c(1, 0, 0, 1);
        return c;
    }

    const color& color::green() noexcept {
        static const color c(0, 1, 0, 1);
        return c;
    }

    const color& color::blue() noexcept {
        static const color c(0, 0, 1, 1);
        return c;
    }

    const color& color::yellow() noexcept {
        static const color c(1, 1, 0, 1);
        return c;
    }

    const color& color::magenta() noexcept {
        static const color c(1, 0, 1, 1);
        return c;
    }

    const color& color::cyan() noexcept {
        static const color c(0, 1, 1, 1);
        return c;
    }

    color::color(const color32& other) noexcept
    : r(other.r / 255.f)
    , g(other.g / 255.f)
    , b(other.b / 255.f)
    , a(other.a / 255.f) {}

    color::color(f32 nr, f32 ng, f32 nb, f32 na) noexcept
    : r(nr)
    , g(ng)
    , b(nb)
    , a(na) {}

    f32* color::data() noexcept {
        return &r;
    }

    const f32* color::data() const noexcept {
        return &r;
    }

    f32& color::operator[](std::size_t index) noexcept {
        E2D_ASSERT(index < 4);
        return data()[index];
    }

    f32 color::operator[](std::size_t index) const noexcept {
        E2D_ASSERT(index < 4);
        return data()[index];
    }

    color& color::operator+=(f32 v) noexcept {
        return *this += color(v,v,v,v);
    }

    color& color::operator-=(f32 v) noexcept {
        return *this -= color(v,v,v,v);
    }

    color& color::operator*=(f32 v) noexcept {
        return *this *= color(v,v,v,v);
    }

    color& color::operator/=(f32 v) noexcept {
        return *this /= color(v,v,v,v);
    }

    color& color::operator+=(const color& other) noexcept {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    color& color::operator-=(const color& other) noexcept {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }

    color& color::operator*=(const color& other) noexcept {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;
        return *this;
    }

    color& color::operator/=(const color& other) noexcept {
        E2D_ASSERT(!math::is_near_zero(other.r, 0.f));
        E2D_ASSERT(!math::is_near_zero(other.g, 0.f));
        E2D_ASSERT(!math::is_near_zero(other.b, 0.f));
        E2D_ASSERT(!math::is_near_zero(other.a, 0.f));
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;
        return *this;
    }
}

namespace e2d
{
    //
    // color (<,==,!=) color
    //

    bool operator<(const color& l, const color& r) noexcept {
        return (l.r < r.r)
            || (l.r == r.r && l.g < r.g)
            || (l.r == r.r && l.g == r.g && l.b < r.b)
            || (l.r == r.r && l.g == r.g && l.b == r.b && l.a < r.a);
    }

    bool operator==(const color& l, const color& r) noexcept {
        return math::approximately(l.r, r.r)
            && math::approximately(l.g, r.g)
            && math::approximately(l.b, r.b)
            && math::approximately(l.a, r.a);
    }

    bool operator!=(const color& l, const color& r) noexcept {
        return !(l == r);
    }

    //
    // color (+,-,*,/) value
    //

    color operator+(color l, f32 v) noexcept {
        l += v;
        return l;
    }

    color operator-(color l, f32 v) noexcept {
        l -= v;
        return l;
    }

    color operator*(color l, f32 v) noexcept {
        l *= v;
        return l;
    }

    color operator/(color l, f32 v) noexcept {
        l /= v;
        return l;
    }

    //
    // value (+,-,*,/) color
    //

    color operator+(f32 v, const color& r) noexcept {
        color l(v,v,v,v);
        l += r;
        return l;
    }

    color operator-(f32 v, const color& r) noexcept {
        color l(v,v,v,v);
        l -= r;
        return l;
    }

    color operator*(f32 v, const color& r) noexcept {
        color l(v,v,v,v);
        l *= r;
        return l;
    }

    color operator/(f32 v, const color& r) noexcept {
        color l(v,v,v,v);
        l /= r;
        return l;
    }

    //
    // color (+,-,*,/) color
    //

    color operator+(color l, const color& r) noexcept {
        l += r;
        return l;
    }

    color operator-(color l, const color& r) noexcept {
        l -= r;
        return l;
    }

    color operator*(color l, const color& r) noexcept {
        l *= r;
        return l;
    }

    color operator/(color l, const color& r) noexcept {
        l /= r;
        return l;
    }
}

namespace e2d { namespace math
{
    //
    // approximately
    //

    bool approximately(const color& l, const color& r) noexcept {
        return math::approximately(l.r, r.r)
            && math::approximately(l.g, r.g)
            && math::approximately(l.b, r.b)
            && math::approximately(l.a, r.a);
    }

    bool approximately(const color& l, const color& r, f32 precision) noexcept {
        return math::approximately(l.r, r.r, precision)
            && math::approximately(l.g, r.g, precision)
            && math::approximately(l.b, r.b, precision)
            && math::approximately(l.a, r.a, precision);
    }

    //
    // minimum/maximum
    //

    f32 minimum(const color& c) noexcept {
        return math::min(math::min(math::min(c.r, c.g), c.b), c.a);
    }

    f32 maximum(const color& c) noexcept {
        return math::max(math::max(math::max(c.r, c.g), c.b), c.a);
    }

    //
    // minimized/maximized/clamped
    //

    color minimized(const color& c, const color& cmin) noexcept {
        return color(
            math::min(c.r, cmin.r),
            math::min(c.g, cmin.g),
            math::min(c.b, cmin.b),
            math::min(c.a, cmin.a));
    }

    color maximized(const color& c, const color& cmax) noexcept {
        return color(
            math::max(c.r, cmax.r),
            math::max(c.g, cmax.g),
            math::max(c.b, cmax.b),
            math::max(c.a, cmax.a));
    }

    color clamped(const color& c, const color& cmin, const color& cmax) noexcept {
        return color(
            math::clamp(c.r, cmin.r, cmax.r),
            math::clamp(c.g, cmin.g, cmax.g),
            math::clamp(c.b, cmin.b, cmax.b),
            math::clamp(c.a, cmin.a, cmax.a));
    }

    color saturated(const color& c) noexcept {
        return clamped(c, color::clear(), color::white());
    }

    //
    // contains_nan
    //

    bool contains_nan(const color& c) noexcept {
        return !math::is_finite(c.r)
            || !math::is_finite(c.g)
            || !math::is_finite(c.b)
            || !math::is_finite(c.a);
    }
}}

namespace e2d { namespace colors
{
    u32 pack_color(const color& c) noexcept {
        return
            math::numeric_cast<u32>(math::round(math::saturate(c.a) * 255.f)) << 24 |
            math::numeric_cast<u32>(math::round(math::saturate(c.r) * 255.f)) << 16 |
            math::numeric_cast<u32>(math::round(math::saturate(c.g) * 255.f)) <<  8 |
            math::numeric_cast<u32>(math::round(math::saturate(c.b) * 255.f)) <<  0;
    }

    color unpack_color(u32 argb) noexcept {
        return color(
            math::numeric_cast<u8>((argb >> 16) & 0xFF) / 255.f,
            math::numeric_cast<u8>((argb >>  8) & 0xFF) / 255.f,
            math::numeric_cast<u8>((argb >>  0) & 0xFF) / 255.f,
            math::numeric_cast<u8>((argb >> 24) & 0xFF) / 255.f);
    }
}}
