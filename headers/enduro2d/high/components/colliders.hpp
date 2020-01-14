/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../gobject.hpp"
#include "../inspector.hpp"

namespace e2d
{
    class rect_collider final {
    public:
        rect_collider() = default;

        rect_collider& size(const v2f& value) noexcept;
        [[nodiscard]] const v2f& size() const noexcept;

        rect_collider& pivot(const v2f& value) noexcept;
        [[nodiscard]] const v2f& pivot() const noexcept;
    private:
        v2f size_ = v2f::zero();
        v2f pivot_ = v2f(0.5f);
    };

    class circle_collider final {
    public:
        circle_collider() = default;

        circle_collider& radius(f32 value) noexcept;
        [[nodiscard]] f32 radius() const noexcept;

        circle_collider& pivot(const v2f& value) noexcept;
        [[nodiscard]] const v2f& pivot() const noexcept;
    private:
        f32 radius_ = 0.f;
        v2f pivot_ = v2f(0.5f);
    };

    class polygon_collider final {
    public:
        polygon_collider() = default;

        polygon_collider& points(vector<v2f> value) noexcept;
        [[nodiscard]] vector<v2f>& points() noexcept;
        [[nodiscard]] const vector<v2f>& points() const noexcept;

        polygon_collider& pivot(const v2f& value) noexcept;
        [[nodiscard]] const v2f& pivot() const noexcept;
    private:
        vector<v2f> points_;
        v2f pivot_ = v2f(0.5f);
    };
}

namespace e2d
{
    template <>
    class factory_loader<rect_collider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            rect_collider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<circle_collider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            circle_collider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<polygon_collider> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            polygon_collider& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<rect_collider> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<rect_collider>& c) const;
    };

    template <>
    class component_inspector<circle_collider> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<circle_collider>& c) const;
    };

    template <>
    class component_inspector<polygon_collider> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<polygon_collider>& c) const;
    };
}

namespace e2d
{
    inline rect_collider& rect_collider::size(const v2f& value) noexcept {
        size_ = value;
        return *this;
    }

    inline const v2f& rect_collider::size() const noexcept {
        return size_;
    }

    inline rect_collider& rect_collider::pivot(const v2f& value) noexcept {
        pivot_ = value;
        return *this;
    }

    inline const v2f& rect_collider::pivot() const noexcept {
        return pivot_;
    }
}

namespace e2d
{
    inline circle_collider& circle_collider::radius(f32 value) noexcept {
        radius_ = value;
        return *this;
    }

    inline f32 circle_collider::radius() const noexcept {
        return radius_;
    }

    inline circle_collider& circle_collider::pivot(const v2f& value) noexcept {
        pivot_ = value;
        return *this;
    }

    inline const v2f& circle_collider::pivot() const noexcept {
        return pivot_;
    }
}

namespace e2d
{
    inline polygon_collider& polygon_collider::points(vector<v2f> value) noexcept {
        points_ = std::move(value);
        return *this;
    }

    inline vector<v2f>& polygon_collider::points() noexcept {
        return points_;
    }

    inline const vector<v2f>& polygon_collider::points() const noexcept {
        return points_;
    }

    inline polygon_collider& polygon_collider::pivot(const v2f& value) noexcept {
        pivot_ = value;
        return *this;
    }

    inline const v2f& polygon_collider::pivot() const noexcept {
        return pivot_;
    }
}
