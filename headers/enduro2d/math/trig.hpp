/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_math.hpp"
#include "unit.hpp"

namespace e2d::math
{
    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        const rad<T>&>
    pi() noexcept {
        static const rad<T> pi = rad<T>(T(3.14159265358979323846264338327950288));
        return pi;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        const rad<T>&>
    inv_pi() noexcept {
        static const rad<T> inv_pi = T(1) / pi<T>();
        return inv_pi;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        const rad<T>&>
    two_pi() noexcept {
        static const rad<T> two_pi = pi<T>() * T(2);
        return two_pi;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        const rad<T>&>
    four_pi() noexcept {
        static const rad<T> four_pi = pi<T>() * T(4);
        return four_pi;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        const rad<T>&>
    half_pi() noexcept {
        static const rad<T> half_pi = pi<T>() / T(2);
        return half_pi;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        const rad<T>&>
    quarter_pi() noexcept {
        static const rad<T> quarter_pi = pi<T>() / T(4);
        return quarter_pi;
    }
}

namespace e2d::math
{
    //
    // deg_to_rad
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    deg_to_rad() noexcept {
        return math::pi<T>().value / T(180);
    }

    template < typename T >
    std::enable_if_t<std::is_integral_v<T>, T>
    deg_to_rad(T deg) noexcept {
        return math::numeric_cast<T>(deg * deg_to_rad<f64>());
    }

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    deg_to_rad(T deg) noexcept {
        return deg * deg_to_rad<T>();
    }

    template < typename T >
    rad<T> deg_to_rad(const deg<T>& deg) noexcept {
        return rad<T>(deg_to_rad(deg.value));
    }

    //
    // rad_to_deg
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    rad_to_deg() noexcept {
        return T(180) / math::pi<T>().value;
    }

    template < typename T >
    std::enable_if_t<std::is_integral_v<T>, T>
    rad_to_deg(T rad) noexcept {
        return math::numeric_cast<T>(rad * rad_to_deg<f64>());
    }

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    rad_to_deg(T rad) noexcept {
        return rad * rad_to_deg<T>();
    }

    template < typename T >
    deg<T> rad_to_deg(const rad<T>& rad) noexcept {
        return deg<T>(rad_to_deg(rad.value));
    }
}

namespace e2d
{
    template < typename T >
    deg<T> make_deg(T v) noexcept {
        return make_unit<deg_tag>(v);
    }

    template < typename T >
    rad<T> make_rad(T v) noexcept {
        return make_unit<rad_tag>(v);
    }

    template <>
    struct unit_converter<rad_tag, deg_tag> {
        template < typename T >
        deg<T> operator()(const rad<T>& u) const noexcept {
            return math::rad_to_deg(u);
        }
    };

    template <>
    struct unit_converter<deg_tag, rad_tag> {
    public:
        template < typename T >
        rad<T> operator()(const deg<T>& u) const noexcept {
            return math::deg_to_rad(u);
        }
    };
}

namespace e2d::math
{
    //
    // to_deg/to_rad
    //

    template < typename T, typename Tag >
    deg<T> to_deg(const unit<T, Tag>& u) noexcept {
        return u.template convert_to<deg_tag>();
    }

    template < typename T, typename Tag >
    T to_deg_v(const unit<T, Tag>& u) noexcept {
        return to_deg(u).value;
    }

    template < typename T, typename Tag >
    rad<T> to_rad(const unit<T, Tag>& u) noexcept {
        return u.template convert_to<rad_tag>();
    }

    template < typename T, typename Tag >
    T to_rad_v(const unit<T, Tag>& u) noexcept {
        return to_rad(u).value;
    }

    //
    // cos/sin/tan
    //

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    cos(const unit<T, Tag>& u) noexcept {
        return std::cos(to_rad(u).value);
    }

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    sin(const unit<T, Tag>& u) noexcept {
        return std::sin(to_rad(u).value);
    }

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, T>
    tan(const unit<T, Tag>& u) noexcept {
        return std::tan(to_rad(u).value);
    }

    //
    // acos, asin, atan
    //

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, rad<T>>
    acos(const unit<T, Tag>& u) noexcept {
        return rad<T>(std::acos(to_rad(u).value));
    }

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, rad<T>>
    asin(const unit<T, Tag>& u) noexcept {
        return rad<T>(std::asin(to_rad(u).value));
    }

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, rad<T>>
    atan(const unit<T, Tag>& u) noexcept {
        return rad<T>(std::atan(to_rad(u).value));
    }

    //
    // atan2
    //

    template < typename T >
    std::enable_if_t<std::is_floating_point_v<T>, rad<T>>
    atan2(T y, T x) noexcept {
        return rad<T>(std::atan2(y, x));
    }

    //
    // normalized_angle
    //

    template < typename T, typename Tag >
    std::enable_if_t<std::is_floating_point_v<T>, unit<T, Tag>>
    normalized_angle(const unit<T, Tag>& u) noexcept {
        const auto four_pi = math::four_pi<T>().template convert_to<Tag>();
        return u - four_pi * math::floor((u + T(0.5) * four_pi).value / four_pi.value);
    }
}
