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
        class dirty final {};
        class was_dirty final {};
    public:
        widget() = default;

        widget& size(const v2f& value) noexcept;
        [[nodiscard]] const v2f& size() const noexcept;

        widget& margin(const v2f& value) noexcept;
        [[nodiscard]] const v2f& margin() const noexcept;

        widget& padding(const v2f& value) noexcept;
        [[nodiscard]] const v2f& padding() const noexcept;
    private:
        v2f size_ = v2f::zero();
        v2f margin_ = v2f::zero();
        v2f padding_ = v2f::zero();
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

    template <>
    class factory_loader<widget::dirty> final
    : public empty_factory_loader<widget::dirty> {};

    template <>
    class factory_loader<widget::was_dirty> final
    : public empty_factory_loader<widget::was_dirty> {};
}

namespace e2d
{
    template <>
    class component_inspector<widget> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<widget>& c) const;
        void operator()(gcomponent<widget>& c, gizmos_context& ctx) const;
    };
}

namespace e2d
{
    inline widget& widget::size(const v2f& value) noexcept {
        size_ = math::maximized(v2f::zero(), value);
        return *this;
    }

    inline const v2f& widget::size() const noexcept {
        return size_;
    }

    inline widget& widget::margin(const v2f& value) noexcept {
        margin_ = math::maximized(v2f::zero(), value);
        return *this;
    }

    inline const v2f& widget::margin() const noexcept {
        return margin_;
    }

    inline widget& widget::padding(const v2f& value) noexcept {
        padding_ = math::maximized(v2f::zero(), value);
        return *this;
    }

    inline const v2f& widget::padding() const noexcept {
        return padding_;
    }
}

namespace e2d::widgets
{
    gcomponent<widget> mark_dirty(gcomponent<widget> self);
    gcomponent<widget> unmark_dirty(gcomponent<widget> self);
    bool is_dirty(const const_gcomponent<widget>& self) noexcept;

    gcomponent<widget> change_size(gcomponent<widget> self, const v2f& value);
    gcomponent<widget> change_margin(gcomponent<widget> self, const v2f& value);
    gcomponent<widget> change_padding(gcomponent<widget> self, const v2f& value);

    gcomponent<layout> find_parent_layout(const_gcomponent<widget> self) noexcept;
}
