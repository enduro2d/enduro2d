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
            draw_line(
                center + p1,
                center + p2,
                color);
        }
    }

    template < typename Container >
    void component_inspector<>::gizmos_context::draw_wire_polygon(
        Container&& container,
        const v2f& center,
        const color32& color)
    {
        draw_wire_polygon(
            std::begin(container),
            std::end(container),
            center,
            color);
    }
}

namespace e2d::impl
{
    template < typename Component >
    void typed_inspector_drawer<Component>::operator()(gobject& go) const {
        if constexpr ( utils::is_detected<has_component_inspector, Component>() ) {
            gcomponent<Component> co = go.component<Component>();
            if ( !co ) {
                return;
            }

            ImGui::PushID(co.find());
            E2D_DEFER([](){ ImGui::PopID(); });

            const bool inspector_opened = ImGui::CollapsingHeader(
                component_inspector<Component>::title,
                ImGuiTreeNodeFlags_AllowItemOverlap);

            const char* component_popup_context_str_id = "e2d_component_popup_context";
            if ( ImGui::BeginPopupContextItem(component_popup_context_str_id) ) {
                E2D_DEFER([](){ ImGui::EndPopup(); });

                auto disabled_co = co.owner().template component<disabled<Component>>();
                if ( bool enabled = !disabled_co; ImGui::Checkbox("Enabled", &enabled) ) {
                    if ( enabled ) {
                        disabled_co.remove();
                    } else {
                        disabled_co.ensure();
                    }
                }

                ImGui::Separator();

                if ( ImGui::Button("Remove") ) {
                    co.remove();
                    return;
                }
            }

            {
                ImGui::SameLine(
                    ImGui::GetWindowContentRegionMax().x -
                    ImGui::GetFrameHeight());

                if ( ImGui::Button(ICON_FA_COG) ) {
                    ImGui::OpenPopup(component_popup_context_str_id);
                }
            }

            if ( inspector_opened ) {
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

            if (  !co.owner().template component<disabled<Component>>() ) {
                inspector_(co, ctx);
            }
        }
    }
}

namespace e2d::impl
{
    template < typename Component >
    void typed_inspector_creator<Component>::ensure(gobject& go) const {
        go.component<Component>().ensure();
    }

    template < typename Component >
    bool typed_inspector_creator<Component>::exists(gobject& go) const noexcept {
        return go.component<Component>().exists();
    }

    template < typename Component >
    const char* typed_inspector_creator<Component>::title() const noexcept {
        return component_inspector<Component>::title;
    }
}

namespace e2d
{
    template < typename Component >
    inspector& inspector::register_component(str type) {
        std::lock_guard<std::mutex> guard(mutex_);

        if ( inspector_impls_.count(type) > 0 ) {
            throw bad_inspector_operation();
        }

        auto drawer = make_intrusive<impl::typed_inspector_drawer<Component>>();
        auto creator = make_intrusive<impl::typed_inspector_creator<Component>>();

        inspector_impls_.insert(std::make_pair(
            std::move(type),
            inspector_impl{
                std::move(drawer),
                std::move(creator)}));

        return *this;
    }
}
