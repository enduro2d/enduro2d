/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    class layout final {
    public:
        class dirty final {};
    public:
        ENUM_HPP_CLASS_DECL(haligns, u8,
            (left)
            (center)
            (right)
            (space_around)
            (space_evenly)
            (space_between))

        ENUM_HPP_CLASS_DECL(valigns, u8,
            (top)
            (center)
            (bottom)
            (space_around)
            (space_evenly)
            (space_between))

        ENUM_HPP_CLASS_DECL(directions, u8,
            (row)
            (row_reversed)
            (column)
            (column_reversed))
    public:
        layout() = default;

        layout& halign(haligns value) noexcept;
        [[nodiscard]] haligns halign() const noexcept;

        layout& valign(valigns value) noexcept;
        [[nodiscard]] valigns valign() const noexcept;

        layout& direction(directions value) noexcept;
        [[nodiscard]] directions direction() const noexcept;
    public:
        layout& size(const v2f& value) noexcept;
        [[nodiscard]] const v2f& size() const noexcept;

        layout& margin(const v2f& value) noexcept;
        [[nodiscard]] const v2f& margin() const noexcept;

        layout& padding(const v2f& value) noexcept;
        [[nodiscard]] const v2f& padding() const noexcept;
    private:
        haligns halign_ = haligns::center;
        valigns valign_ = valigns::center;
        directions direction_ = directions::row;
    private:
        v2f size_ = v2f::zero();
        v2f margin_ = v2f::zero();
        v2f padding_ = v2f::zero();
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::layout::haligns)
ENUM_HPP_REGISTER_TRAITS(e2d::layout::valigns)
ENUM_HPP_REGISTER_TRAITS(e2d::layout::directions)

namespace e2d
{
    template <>
    class factory_loader<layout> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            layout& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<layout::dirty> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            layout::dirty& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<layout> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<layout>& c) const;
        void operator()(gcomponent<layout>& c, gizmos_context& ctx) const;
    };
}

namespace e2d
{
    inline layout& layout::halign(haligns value) noexcept {
        halign_ = value;
        return *this;
    }

    inline layout::haligns layout::halign() const noexcept {
        return halign_;
    }

    inline layout& layout::valign(valigns value) noexcept {
        valign_ = value;
        return *this;
    }

    inline layout::valigns layout::valign() const noexcept {
        return valign_;
    }

    inline layout& layout::direction(directions value) noexcept {
        direction_ = value;
        return *this;
    }

    inline layout::directions layout::direction() const noexcept {
        return direction_;
    }

    inline layout& layout::size(const v2f& value) noexcept {
        size_ = value;
        return *this;
    }

    inline const v2f& layout::size() const noexcept {
        return size_;
    }

    inline layout& layout::margin(const v2f& value) noexcept {
        margin_ = value;
        return *this;
    }

    inline const v2f& layout::margin() const noexcept {
        return margin_;
    }

    inline layout& layout::padding(const v2f& value) noexcept {
        padding_ = value;
        return *this;
    }

    inline const v2f& layout::padding() const noexcept {
        return padding_;
    }
}

namespace e2d::layouts
{
    gcomponent<layout> mark_dirty(gcomponent<layout> self);
    gcomponent<layout> unmark_dirty(gcomponent<layout> self);
    bool is_dirty(const const_gcomponent<layout>& self) noexcept;

    gcomponent<layout> change_halign(gcomponent<layout> self, layout::haligns value);
    gcomponent<layout> change_valign(gcomponent<layout> self, layout::valigns value);
    gcomponent<layout> change_direction(gcomponent<layout> self, layout::directions value);

    gcomponent<layout> change_size(gcomponent<layout> self, const v2f& value);
    gcomponent<layout> change_margin(gcomponent<layout> self, const v2f& value);
    gcomponent<layout> change_padding(gcomponent<layout> self, const v2f& value);

    gcomponent<layout> find_parent_layout(const_gcomponent<layout> self) noexcept;
}
