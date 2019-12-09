/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    template < typename T >
    struct property_traits {
        using step_t = T;
        using range_t = std::pair<T,T>;
    };

    template < typename T >
    struct property_traits<vec2<T>> {
        using step_t = T;
        using range_t = std::pair<T,T>;
    };

    template < typename T >
    struct property_traits<vec3<T>> {
        using step_t = T;
        using range_t = std::pair<T,T>;
    };

    template < typename T >
    struct property_traits<vec4<T>> {
        using step_t = T;
        using range_t = std::pair<T,T>;
    };
}

namespace e2d
{
    template < typename T >
    class property {
    public:
        property() = delete;

        property(property&&) = default;
        property& operator=(property&&) = default;

        property(const property&) = default;
        property& operator=(const property&) = default;

        property(T value)
            noexcept(std::is_nothrow_move_constructible_v<T>)
        : value_(std::move(value)) {}

        property& operator=(T value)
            noexcept(std::is_nothrow_move_assignable_v<T>)
        {
            value_ = std::move(value);
            return *this;
        }

        const T& operator*() const noexcept {
            return value_;
        }

        const T* operator->() const noexcept {
            return &value_;
        }
    private:
        T value_{};
    };
}

namespace e2d
{
    class text_property {
    public:
        text_property() = delete;

        text_property(text_property&&) = default;
        text_property& operator=(text_property&&) = default;

        text_property(const text_property&) = default;
        text_property& operator=(const text_property&) = default;

        text_property(str value) noexcept
        : value_(std::move(value)) {}

        text_property(str value, bool multiline) noexcept
        : value_(std::move(value))
        , multiline_(multiline) {}

        property<str>& value() noexcept {
            return value_;
        }

        const property<str>& value() const noexcept {
            return value_;
        }

        bool multiline() const noexcept {
            return multiline_;
        }
    private:
        property<str> value_;
        bool multiline_ = false;
    };
}

namespace e2d
{
    template < typename T >
    class drag_property {
    public:
        using step_t = typename property_traits<T>::step_t;
        using range_t = typename property_traits<T>::range_t;
    public:
        drag_property() = delete;

        drag_property(drag_property&&) = default;
        drag_property& operator=(drag_property&&) = default;

        drag_property(const drag_property&) = default;
        drag_property& operator=(const drag_property&) = default;

        drag_property(T value)
            noexcept(std::is_nothrow_move_constructible_v<T>)
        : value_(std::move(value)) {}

        drag_property(T value, step_t step)
            noexcept(std::is_nothrow_move_constructible_v<T>
                && std::is_nothrow_move_constructible_v<step_t>)
        : value_(std::move(value))
        , step_(std::move(step)) {}

        drag_property(T value, step_t step, range_t range)
            noexcept(std::is_nothrow_move_constructible_v<T>
                && std::is_nothrow_move_constructible_v<step_t>
                && std::is_nothrow_move_constructible_v<range_t>)
        : value_(std::move(value))
        , step_(std::move(step))
        , range_(std::move(range)) {}

        property<T>& value() noexcept {
            return value_;
        }

        const property<T>& value() const noexcept {
            return value_;
        }

        const std::optional<step_t>& step() const noexcept {
            return step_;
        }

        const std::optional<range_t>& range() const noexcept {
            return range_;
        }
    private:
        property<T> value_;
        std::optional<step_t> step_;
        std::optional<range_t> range_;
    };
}

namespace e2d
{
    template < typename T >
    class slider_property {
    public:
        using range_t = typename property_traits<T>::range_t;
    public:
        slider_property() = delete;

        slider_property(slider_property&&) = default;
        slider_property& operator=(slider_property&&) = default;

        slider_property(const slider_property&) = default;
        slider_property& operator=(const slider_property&) = default;

        slider_property(T value, range_t range)
            noexcept(std::is_nothrow_move_constructible_v<T>
                && std::is_nothrow_move_constructible_v<range_t>)
        : value_(std::move(value))
        , range_(std::move(range)) {}

        property<T>& value() noexcept {
            return value_;
        }

        const property<T>& value() const noexcept {
            return value_;
        }

        const range_t& range() const noexcept {
            return range_;
        }
    private:
        property<T> value_;
        range_t range_{};
    };
}
