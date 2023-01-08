/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class slider final {
    public:
        ENUM_HPP_CLASS_DECL(directions, u8,
            (row)
            (row_reversed)
            (column)
            (column_reversed))
    public:
        slider() = default;

        slider& min_value(f32 value) noexcept;
        [[nodiscard]] f32 min_value() const noexcept;

        slider& max_value(f32 value) noexcept;
        [[nodiscard]] f32 max_value() const noexcept;

        slider& value(f32 value) noexcept;
        [[nodiscard]] f32 value() const noexcept;

        slider& raw_value(f32 value) noexcept;
        [[nodiscard]] f32 raw_value() const noexcept;

        slider& normalized_value(f32 value) noexcept;
        [[nodiscard]] f32 normalized_value() const noexcept;

        slider& normalized_raw_value(f32 value) noexcept;
        [[nodiscard]] f32 normalized_raw_value() const noexcept;

        slider& whole_numbers(bool value) noexcept;
        [[nodiscard]] bool whole_numbers() const noexcept;

        slider& direction(directions value) noexcept;
        [[nodiscard]] directions direction() const noexcept;
    private:
        f32 min_value_ = 0.f;
        f32 max_value_ = 1.f;
        f32 value_ = 0.f;
        bool whole_numbers_ = false;
        directions direction_ = directions::row;
    };

    ENUM_HPP_REGISTER_TRAITS(slider::directions)
}

namespace e2d
{
    template <>
    class factory_loader<slider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            slider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<slider> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<slider>& c) const;
    };
}

namespace e2d
{
    inline slider& slider::min_value(f32 value) noexcept {
        min_value_ = whole_numbers_ ? math::floor(value) : value;
        max_value_ = math::max(max_value_, min_value_);
        value_ = math::clamp(value_, min_value_, max_value_);
        return *this;
    }

    inline f32 slider::min_value() const noexcept {
        return min_value_;
    }

    inline slider& slider::max_value(f32 value) noexcept {
        max_value_ = whole_numbers_ ? math::ceil(value) : value;
        min_value_ = math::min(min_value_, max_value_);
        value_ = math::clamp(value_, min_value_, max_value_);
        return *this;
    }

    inline f32 slider::max_value() const noexcept {
        return max_value_;
    }

    inline slider& slider::value(f32 value) noexcept {
        value_ = math::clamp(value, min_value_, max_value_);
        return *this;
    }

    inline f32 slider::value() const noexcept {
        return whole_numbers_ ? math::round(value_) : value_;
    }

    inline slider& slider::raw_value(f32 value) noexcept {
        value_ = math::clamp(value, min_value_, max_value_);
        return *this;
    }

    inline f32 slider::raw_value() const noexcept {
        return value_;
    }

    inline slider& slider::normalized_value(f32 value) noexcept {
        value_ = math::lerp(
            min_value_,
            max_value_,
            math::saturate(value));
        return *this;
    }

    inline f32 slider::normalized_value() const noexcept {
        return math::approximately(min_value_, max_value_)
            ? 0.f
            : math::inverse_lerp(min_value_, max_value_, value());
    }

    inline slider& slider::normalized_raw_value(f32 value) noexcept {
        value_ = math::lerp(
            min_value_,
            max_value_,
            math::saturate(value));
        return *this;
    }

    inline f32 slider::normalized_raw_value() const noexcept {
        return math::approximately(min_value_, max_value_)
            ? 0.f
            : math::inverse_lerp(min_value_, max_value_, raw_value());
    }

    inline slider& slider::whole_numbers(bool value) noexcept {
        whole_numbers_ = value;
        if ( whole_numbers_ ) {
            min_value_ = math::floor(min_value_);
            max_value_ = math::ceil(max_value_);
            value_ = math::clamp(value_, min_value_, max_value_);
        }
        return *this;
    }

    inline bool slider::whole_numbers() const noexcept {
        return whole_numbers_;
    }

    inline slider& slider::direction(directions value) noexcept {
        direction_ = value;
        return *this;
    }

    inline slider::directions slider::direction() const noexcept {
        return direction_;
    }
}