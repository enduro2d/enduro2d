/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class camera final {
    public:
        class input final {};
        class gizmos final {};
    public:
        ENUM_HPP_CLASS_DECL(modes, u8,
            (manual)
            (stretch)
            (flexible)
            (fixed_fit)
            (fixed_crop))
    public:
        camera() = default;

        camera& depth(i32 value) noexcept;
        camera& mode(modes value) noexcept;
        camera& znear(f32 value) noexcept;
        camera& zfar(f32 value) noexcept;
        camera& view(const m4f& value) noexcept;
        camera& viewport(const b2f& value) noexcept;
        camera& projection(const m4f& value) noexcept;
        camera& target(const render_target_ptr& value) noexcept;
        camera& background(const color& value) noexcept;

        [[nodiscard]] i32 depth() const noexcept;
        [[nodiscard]] modes mode() const noexcept;
        [[nodiscard]] f32 znear() const noexcept;
        [[nodiscard]] f32 zfar() const noexcept;
        [[nodiscard]] const m4f& view() const noexcept;
        [[nodiscard]] const b2f& viewport() const noexcept;
        [[nodiscard]] const m4f& projection() const noexcept;
        [[nodiscard]] const render_target_ptr& target() const noexcept;
        [[nodiscard]] const color& background() const noexcept;
    private:
        i32 depth_ = 0;
        modes mode_ = modes::flexible;
        f32 znear_ = 0.f;
        f32 zfar_ = 1000.f;
        m4f view_ = m4f::identity();
        b2f viewport_ = b2f::unit();
        m4f projection_ = m4f::identity();
        render_target_ptr target_ = nullptr;
        color background_ = color::clear();
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::camera::modes)

namespace e2d
{
    template <>
    class factory_loader<camera> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            camera& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<camera::input> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            camera::input& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<camera::gizmos> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            camera::gizmos& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<camera> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<camera>& c) const;
    };
}

namespace e2d
{
    inline camera& camera::depth(i32 value) noexcept {
        depth_ = value;
        return *this;
    }

    inline camera& camera::mode(modes value) noexcept {
        mode_ = value;
        return *this;
    }

    inline camera& camera::znear(f32 value) noexcept {
        znear_ = value;
        return *this;
    }

    inline camera& camera::zfar(f32 value) noexcept {
        zfar_ = value;
        return *this;
    }

    inline camera& camera::view(const m4f& value) noexcept {
        view_ = value;
        return *this;
    }

    inline camera& camera::viewport(const b2f& value) noexcept {
        viewport_ = value;
        return *this;
    }

    inline camera& camera::projection(const m4f& value) noexcept {
        projection_ = value;
        return *this;
    }

    inline camera& camera::target(const render_target_ptr& value) noexcept {
        target_ = value;
        return *this;
    }

    inline camera& camera::background(const color& value) noexcept {
        background_ = value;
        return *this;
    }

    inline i32 camera::depth() const noexcept {
        return depth_;
    }

    inline camera::modes camera::mode() const noexcept {
        return mode_;
    }

    inline f32 camera::znear() const noexcept {
        return znear_;
    }

    inline f32 camera::zfar() const noexcept {
        return zfar_;
    }

    inline const m4f& camera::view() const noexcept {
        return view_;
    }

    inline const b2f& camera::viewport() const noexcept {
        return viewport_;
    }

    inline const m4f& camera::projection() const noexcept {
        return projection_;
    }

    inline const render_target_ptr& camera::target() const noexcept {
        return target_;
    }

    inline const color& camera::background() const noexcept {
        return background_;
    }
}
