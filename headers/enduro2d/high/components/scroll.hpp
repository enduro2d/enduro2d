/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class scroll final {
    public:
        scroll() = default;

        scroll& vertical(bool value) noexcept;
        [[nodiscard]] bool vertical() const noexcept;

        scroll& horizontal(bool value) noexcept;
        [[nodiscard]] bool horizontal() const noexcept;

        scroll& inertia(f32 value) noexcept;
        [[nodiscard]] f32 inertia() const noexcept;

        scroll& elasticity(f32 value) noexcept;
        [[nodiscard]] f32 elasticity() const noexcept;

        scroll& sensitivity(f32 value) noexcept;
        [[nodiscard]] f32 sensitivity() const noexcept;
    private:
        bool vertical_ = true;
        bool horizontal_ = true;
        f32 inertia_ = 0.135f;
        f32 elasticity_ = 0.1f;
        f32 sensitivity_ = 1.f;
    };
}

namespace e2d
{
    template <>
    class factory_loader<scroll> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            scroll& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<scroll> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<scroll>& c) const;
    };
}

namespace e2d
{
    inline scroll& scroll::vertical(bool value) noexcept {
        vertical_ = value;
        return *this;
    }

    inline bool scroll::vertical() const noexcept {
        return vertical_;
    }

    inline scroll& scroll::horizontal(bool value) noexcept {
        horizontal_ = value;
        return *this;
    }

    inline bool scroll::horizontal() const noexcept {
        return horizontal_;
    }

    inline scroll& scroll::inertia(f32 value) noexcept {
        inertia_ = math::saturate(value);
        return *this;
    }

    inline f32 scroll::inertia() const noexcept {
        return inertia_;
    }

    inline scroll& scroll::elasticity(f32 value) noexcept {
        elasticity_ = math::saturate(value);
        return *this;
    }

    inline f32 scroll::elasticity() const noexcept {
        return elasticity_;
    }

    inline scroll& scroll::sensitivity(f32 value) noexcept {
        sensitivity_ = math::max(value, 0.f);
        return *this;
    }

    inline f32 scroll::sensitivity() const noexcept {
        return sensitivity_;
    }
}
