/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class widget final {
    public:
        widget() = default;

        widget& size(const v2f& value) noexcept;
        [[nodiscard]] const v2f& size() const noexcept;

        widget& pivot(const v2f& value) noexcept;
        [[nodiscard]] const v2f& pivot() const noexcept;
    private:
        v2f size_ = v2f::zero();
        v2f pivot_ = v2f::unit() * 0.5f;
    };
}

namespace e2d
{
    template <>
    class factory_loader<widget> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            widget& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<widget> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<widget>& c) const;
    };
}

namespace e2d
{
    inline widget& widget::size(const v2f& value) noexcept {
        size_ = value;
        return *this;
    }

    inline const v2f& widget::size() const noexcept {
        return size_;
    }

    inline widget& widget::pivot(const v2f& value) noexcept {
        pivot_ = value;
        return *this;
    }

    inline const v2f& widget::pivot() const noexcept {
        return pivot_;
    }
}
