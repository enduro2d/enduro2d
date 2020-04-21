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
        ENUM_HPP_CLASS_DECL(directions, u8,
            (ltr)
            (rtl))

        ENUM_HPP_CLASS_DECL(align_modes, u8,
            (flex_start)
            (center)
            (flex_end)
            (space_between)
            (space_around))

        ENUM_HPP_CLASS_DECL(justify_modes, u8,
            (flex_start)
            (center)
            (flex_end)
            (space_between)
            (space_around)
            (space_evenly))

        ENUM_HPP_CLASS_DECL(flex_wraps, u8,
            (no_wrap)
            (wrap)
            (wrap_reversed))

        ENUM_HPP_CLASS_DECL(flex_directions, u8,
            (row)
            (row_reversed)
            (column)
            (column_reversed))
    public:
        layout() = default;

        layout& direction(directions value) noexcept;
        [[nodiscard]] directions direction() const noexcept;

        layout& align_items(align_modes value) noexcept;
        [[nodiscard]] align_modes align_items() const noexcept;

        layout& align_content(align_modes value) noexcept;
        [[nodiscard]] align_modes align_content() const noexcept;

        layout& justify_content(justify_modes value) noexcept;
        [[nodiscard]] justify_modes justify_content() const noexcept;

        layout& flex_wrap(flex_wraps value) noexcept;
        [[nodiscard]] flex_wraps flex_wrap() const noexcept;

        layout& flex_direction(flex_directions value) noexcept;
        [[nodiscard]] flex_directions flex_direction() const noexcept;
    private:
        directions direction_ = directions::ltr;
        align_modes align_items_ = align_modes::flex_start;
        align_modes align_content_ = align_modes::flex_start;
        justify_modes justify_content_ = justify_modes::flex_start;
        flex_wraps flex_wrap_ = flex_wraps::no_wrap;
        flex_directions flex_direction_ = flex_directions::row;
    };

    ENUM_HPP_REGISTER_TRAITS(layout::directions)
    ENUM_HPP_REGISTER_TRAITS(layout::align_modes)
    ENUM_HPP_REGISTER_TRAITS(layout::justify_modes)
    ENUM_HPP_REGISTER_TRAITS(layout::flex_wraps)
    ENUM_HPP_REGISTER_TRAITS(layout::flex_directions)
}

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
    };
}

namespace e2d
{
    inline layout& layout::direction(directions value) noexcept {
        direction_ = value;
        return *this;
    }

    inline layout::directions layout::direction() const noexcept {
        return direction_;
    }

    inline layout& layout::align_items(align_modes value) noexcept {
        align_items_ = value;
        return *this;
    }

    inline layout::align_modes layout::align_items() const noexcept {
        return align_items_;
    }

    inline layout& layout::align_content(align_modes value) noexcept {
        align_content_ = value;
        return *this;
    }

    inline layout::align_modes layout::align_content() const noexcept {
        return align_content_;
    }

    inline layout& layout::justify_content(justify_modes value) noexcept {
        justify_content_ = value;
        return *this;
    }

    inline layout::justify_modes layout::justify_content() const noexcept {
        return justify_content_;
    }

    inline layout& layout::flex_wrap(flex_wraps value) noexcept {
        flex_wrap_ = value;
        return *this;
    }

    inline layout::flex_wraps layout::flex_wrap() const noexcept {
        return flex_wrap_;
    }

    inline layout& layout::flex_direction(flex_directions value) noexcept {
        flex_direction_ = value;
        return *this;
    }

    inline layout::flex_directions layout::flex_direction() const noexcept {
        return flex_direction_;
    }
}

namespace e2d::layouts
{
    gcomponent<layout> mark_dirty(gcomponent<layout> self);
    gcomponent<layout> unmark_dirty(gcomponent<layout> self);
    bool is_dirty(const const_gcomponent<layout>& self) noexcept;

    gcomponent<layout> change_direction(gcomponent<layout> self, layout::directions value);
    gcomponent<layout> change_align_items(gcomponent<layout> self, layout::align_modes value);
    gcomponent<layout> change_align_content(gcomponent<layout> self, layout::align_modes value);
    gcomponent<layout> change_justify_content(gcomponent<layout> self, layout::justify_modes value);
    gcomponent<layout> change_flex_wrap(gcomponent<layout> self, layout::flex_wraps value);
    gcomponent<layout> change_flex_direction(gcomponent<layout> self, layout::flex_directions value);

    gcomponent<layout> find_parent_layout(const_gcomponent<layout> self) noexcept;
}
