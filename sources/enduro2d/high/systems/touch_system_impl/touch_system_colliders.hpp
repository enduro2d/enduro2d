/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "touch_system_base.hpp"

namespace e2d::touch_system_impl
{
    namespace impl
    {
        void update_world_space_collider(
            world_space_rect_collider& dst,
            const rect_collider& src,
            const m4f& local_to_world);

        void update_world_space_collider(
            world_space_circle_collider& dst,
            const circle_collider& src,
            const m4f& local_to_world);

        void update_world_space_collider(
            world_space_polygon_collider& dst,
            const polygon_collider& src,
            const m4f& local_to_world);

        template < typename WorldSpaceCollider >
        void update_world_space_colliders(ecs::registry& owner) {
            using world_space_collider_t = WorldSpaceCollider;
            using local_space_collider_t = typename WorldSpaceCollider::local_space_collider_t;

            ecsex::remove_all_components<world_space_collider_t>(
                owner,
                !ecs::exists_all<
                    touchable,
                    local_space_collider_t>() ||
                ecs::exists_any<
                    disabled<actor>,
                    disabled<touchable>,
                    disabled<world_space_collider_t>,
                    disabled<local_space_collider_t>>());

            owner.for_joined_components<local_space_collider_t, touchable, actor>([](
                ecs::entity e,
                const local_space_collider_t& src,
                const touchable&,
                const actor& a)
            {
                update_world_space_collider(
                    e.ensure_component<world_space_collider_t>(),
                    src,
                    a.node() ? a.node()->world_matrix() : m4f::identity());
            }, !ecs::exists_any<
                disabled<actor>,
                disabled<touchable>,
                disabled<world_space_collider_t>,
                disabled<local_space_collider_t>>());
        }
    }

    namespace impl
    {
        bool is_world_space_collider_under_mouse(
            const world_space_rect_collider& c,
            const v2f& mouse_p,
            const m4f& camera_vp,
            const b2f& camera_viewport);

        bool is_world_space_collider_under_mouse(
            const world_space_circle_collider& c,
            const v2f& mouse_p,
            const m4f& camera_vp,
            const b2f& camera_viewport);

        bool is_world_space_collider_under_mouse(
            const world_space_polygon_collider& c,
            const v2f& mouse_p,
            const m4f& camera_vp,
            const b2f& camera_viewport);

        template < typename WorldSpaceCollider >
        void update_world_space_colliders_under_mouse(
            ecs::registry& owner,
            const v2f& mouse_p,
            const m4f& camera_vp,
            const b2f& camera_viewport)
        {
            using world_space_collider_t = WorldSpaceCollider;
            using local_space_collider_t = typename WorldSpaceCollider::local_space_collider_t;

            owner.for_joined_components<touchable, world_space_collider_t>([
                &mouse_p,
                &camera_vp,
                &camera_viewport
            ](ecs::entity e, const touchable&, const world_space_collider_t& c){
                if ( is_world_space_collider_under_mouse(c, mouse_p, camera_vp, camera_viewport) ) {
                    e.ensure_component<touchable_under_mouse>();
                }
            }, !ecs::exists_any<
                disabled<touchable>,
                disabled<world_space_collider_t>,
                disabled<local_space_collider_t>>());
        }
    }

    void update_world_space_colliders(ecs::registry& owner);
    void update_world_space_colliders_under_mouse(input& input, window& window, ecs::registry& owner);
}
