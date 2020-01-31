/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "inspector.hpp"

namespace e2d
{
    inline void component_inspector<>::gizmos_context::draw_rect(
        const v2f& center,
        const v2f& size,
        const color32& color)
    {
        const v2f& of = center;
        const v2f& hs = size * 0.5f;
        draw_rect(
            {of.x - hs.x, of.y - hs.y},
            {of.x + hs.x, of.y - hs.y},
            {of.x + hs.x, of.y + hs.y},
            {of.x - hs.x, of.y + hs.y},
            color);
    }

    inline void component_inspector<>::gizmos_context::draw_wire_rect(
        const v2f& center,
        const v2f& size,
        const color32& color)
    {
        const v2f& of = center;
        const v2f& hs = size * 0.5f;
        draw_wire_rect(
            {of.x - hs.x, of.y - hs.y},
            {of.x + hs.x, of.y - hs.y},
            {of.x + hs.x, of.y + hs.y},
            {of.x - hs.x, of.y + hs.y},
            color);
    }

    template < typename Iter >
    void component_inspector<>::gizmos_context::draw_wire_polygon(
        Iter begin,
        Iter end,
        const v2f& center,
        const color32& color)
    {
        for ( Iter iter = begin; iter != end; ) {
            const v2f p1 = *iter++;
            const v2f p2 = (iter != end) ? *iter : *begin;
            draw_line(center + p1, center + p2, color);
        }
    }

    template < typename Container >
    void component_inspector<>::gizmos_context::draw_wire_polygon(
        Container&& container,
        const v2f& center,
        const color32& color)
    {
        draw_wire_polygon(std::begin(container), std::end(container), center, color);
    }
}

namespace e2d::impl
{
    template < typename Component >
    void typed_inspector_drawer<Component>::operator()(
        gobject& go) const
    {
        if constexpr ( utils::is_detected<has_component_inspector, Component>() ) {
            gcomponent<Component> co = go.component<Component>();
            if ( !co ) {
                return;
            }

            ImGui::PushID(co.find());
            E2D_DEFER([](){ ImGui::PopID(); });

            if ( ImGui::CollapsingHeader(component_inspector<Component>::title) ) {
                inspector_(co);
            }
        }
    }

    template < typename Component >
    void typed_inspector_drawer<Component>::operator()(
        gobject& go,
        component_inspector<>::gizmos_context& ctx) const
    {
        if constexpr ( utils::is_detected<has_component_inspector_gizmos, Component>() ) {
            gcomponent<Component> co = go.component<Component>();
            if ( !co ) {
                return;
            }

            ImGui::PushID(co.find());
            E2D_DEFER([](){ ImGui::PopID(); });

            inspector_(co, ctx);
        }
    }
}

namespace e2d
{
    template < typename Component >
    inspector& inspector::register_component(str_hash type) {
        std::lock_guard<std::mutex> guard(mutex_);
        if ( drawers_.count(type) > 0 ) {
            throw bad_inspector_operation();
        }
        impl::inspector_drawer_iptr drawer(new impl::typed_inspector_drawer<Component>());
        drawers_.emplace(type, std::move(drawer));
        return *this;
    }
}
