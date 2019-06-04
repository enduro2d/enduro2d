/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"

namespace e2d
{
    template < typename T, typename Tag >
    class unit final {
        static_assert(
            std::is_arithmetic_v<T>,
            "type of 'unit' must be arithmetic");
    public:
        using self_type = unit;
        using value_type = T;
        using tag_type = Tag;
    public:
        T value = 0;
    public:
        unit() noexcept = default;
        unit(const unit& other) noexcept = default;
        unit& operator=(const unit& other) noexcept = default;

        explicit unit(T v) noexcept;
        unit(T v, Tag tag) noexcept;
        template < typename OtherTag >
        explicit unit(const unit<T, OtherTag>& other) noexcept;

        template < typename To >
        unit<To, Tag> cast_to() const noexcept;

        template < typename OtherTag >
        unit<T, OtherTag> convert_to() const noexcept;

        unit& operator*=(T v) noexcept;
        unit& operator/=(T v) noexcept;

        unit& operator+=(const unit& other) noexcept;
        unit& operator-=(const unit& other) noexcept;
    };

    template < typename FromTag, typename ToTag >
    struct unit_converter;

    template < typename Tag >
    struct unit_converter<Tag, Tag> {
        template < typename T >
        unit<T, Tag> operator()(const unit<T, Tag>& u) const noexcept {
            return u;
        }
    };
}

namespace e2d
{
    template < typename T, typename Tag >
    unit<T, Tag>::unit(T v) noexcept
    : value(v) {}

    template < typename T, typename Tag >
    unit<T, Tag>::unit(T v, Tag tag) noexcept
    : value(v) { E2D_UNUSED(tag); }

    template < typename T, typename Tag >
    template < typename OtherTag >
    unit<T, Tag>::unit(const unit<T, OtherTag>& other) noexcept
    : value(other.template convert_to<Tag>().value) {}

    template < typename T, typename Tag >
    template < typename To >
    unit<To, Tag> unit<T, Tag>::cast_to() const noexcept {
        return unit<To, Tag>(math::numeric_cast<To>(value));
    }

    template < typename T, typename Tag >
    template < typename OtherTag >
    unit<T, OtherTag> unit<T, Tag>::convert_to() const noexcept {
        return unit_converter<Tag, OtherTag>()(*this);
    }

    template < typename T, typename Tag >
    unit<T, Tag>& unit<T, Tag>::operator*=(T v) noexcept {
        value *= v;
        return *this;
    }

    template < typename T, typename Tag >
    unit<T, Tag>& unit<T, Tag>::operator/=(T v) noexcept {
        E2D_ASSERT(!math::is_near_zero(v, T(0)));
        value /= v;
        return *this;
    }

    template < typename T, typename Tag >
    unit<T, Tag>& unit<T, Tag>::operator+=(const unit& other) noexcept {
        value += other.value;
        return *this;
    }

    template < typename T, typename Tag >
    unit<T, Tag>& unit<T, Tag>::operator-=(const unit& other) noexcept {
        value -= other.value;
        return *this;
    }
}

namespace e2d
{
    //
    // make_unit
    //

    template < typename Tag, typename T >
    unit<T, Tag> make_unit(T v) noexcept {
        return unit<T, Tag>(v);
    }

    //
    // unit (==,!=) unit
    //

    template < typename T, typename Tag >
    bool operator==(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return math::approximately(l.value, r.value);
    }

    template < typename T, typename Tag  >
    bool operator!=(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return !(l == r);
    }

    //
    // unit (<,>,<=,>=) unit
    //

    template < typename T, typename Tag >
    bool operator<(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return l.value < r.value;
    }

    template < typename T, typename Tag >
    bool operator>(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return l.value > r.value;
    }

    template < typename T, typename Tag >
    bool operator<=(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return l.value <= r.value;
    }

    template < typename T, typename Tag >
    bool operator>=(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return l.value >= r.value;
    }

    //
    // (-) unit
    //

    template < typename T, typename Tag >
    unit<T, Tag> operator-(const unit<T, Tag>& u) noexcept {
        return {
            -u.value,
            Tag{}};
    }

    //
    // unit (*,/) value
    //

    template < typename T, typename Tag >
    unit<T, Tag> operator*(const unit<T, Tag>& l, T v) noexcept {
        return {
            l.value * v,
            Tag{}};
    }

    template < typename T, typename Tag >
    unit<T, Tag> operator/(const unit<T, Tag>& l, T v) noexcept {
        E2D_ASSERT(!math::is_near_zero(v, T(0)));
        return {
            l.value / v,
            Tag{}};
    }

    //
    // value (*,/) unit
    //

    template < typename T, typename Tag >
    unit<T, Tag> operator*(T v, const unit<T, Tag>& r) noexcept {
        return {
            v * r.value,
            Tag{}};
    }

    template < typename T, typename Tag >
    unit<T, Tag> operator/(T v, const unit<T, Tag>& r) noexcept {
        E2D_ASSERT(!math::approximately(r.value, T(0), T(0)));
        return {
            v / r.value,
            Tag{}};
    }

    //
    // unit (+,-) unit
    //

    template < typename T, typename Tag >
    unit<T, Tag> operator+(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return {
            l.value + r.value,
            Tag{}};
    }

    template < typename T, typename Tag >
    unit<T, Tag> operator-(const unit<T, Tag>& l, const unit<T, Tag>& r) noexcept {
        return {
            l.value - r.value,
            Tag{}};
    }
}

namespace e2d::math
{
    //
    // approximately
    //

    template < typename T, typename Tag >
    bool approximately(
        const unit<T, Tag>& l,
        const unit<T, Tag>& r,
        T precision = math::default_precision<T>()) noexcept
    {
        return math::approximately(l.value, r.value, precision);
    }

    //
    // minimized/maximized/clamped
    //

    template < typename T, typename Tag >
    unit<T, Tag> minimized(const unit<T, Tag>& u, const unit<T, Tag>& umin) noexcept {
        return {
            math::min(u.value, umin.value),
            Tag{}};
    }

    template < typename T, typename Tag >
    unit<T, Tag> maximized(const unit<T, Tag>& u, const unit<T, Tag>& umax) noexcept {
        return {
            math::max(u.value, umax.value),
            Tag{}};
    }

    template < typename T, typename Tag >
    unit<T, Tag> clamped(const unit<T, Tag>& u, const unit<T, Tag>& umin, const unit<T, Tag>& umax) noexcept {
        return {
            math::clamp(u.value, umin.value, umax.value),
            Tag{}};
    }

    template < typename T, typename Tag >
    unit<T, Tag> saturated(const unit<T, Tag>& u) noexcept {
        return clamped(u, {T(0), Tag{}}, {T(1), Tag{}});
    }

    //
    // contains_nan
    //

    template < typename T, typename Tag >
    bool contains_nan(const unit<T, Tag>& u) noexcept {
        return !math::is_finite(u.value);
    }
}
