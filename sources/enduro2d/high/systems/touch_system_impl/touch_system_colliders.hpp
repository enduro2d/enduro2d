/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "touch_system_base.hpp"

namespace e2d::touch_system_impl
{
    namespace impl
    {
        void update_world_space_collider_points(
            world_space_rect_collider& dst,
            const rect_collider& src);

        void update_world_space_collider_points(
            world_space_circle_collider& dst,
            const circle_collider& src);

        void update_world_space_collider_points(
            world_space_polygon_collider& dst,
            const polygon_collider& src);

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
                world_space_collider_t& world_space_collider =
                    e.ensure_component<world_space_collider_t>();

                world_space_collider.local_to_world =
                    a.node() ? a.node()->world_matrix() : m4f::identity();

                update_world_space_collider_points(world_space_collider, src);
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

            m4f inv_camera_vp{m4f::identity()};
            bool inv_camera_vp_success{false};

            std::tie(inv_camera_vp, inv_camera_vp_success) = math::inversed(camera_vp, 0.f);
            if ( !inv_camera_vp_success ) {
                return;
            }

            owner.for_joined_components<touchable, world_space_collider_t>([
                &mouse_p,
                &camera_vp,
                &inv_camera_vp,
                &camera_viewport
            ](ecs::entity e, const touchable&, const world_space_collider_t& c){
                if ( is_world_space_collider_under_mouse(c, mouse_p, camera_vp, camera_viewport) ) {
                    const auto& [world_point, world_point_success] = math::unproject(
                        v3f(mouse_p, 0.f),
                        inv_camera_vp,
                        camera_viewport);

                    if ( !world_point_success ) {
                        return;
                    }

                    const auto& [world_to_local, world_to_local_success] = math::inversed(
                        c.local_to_world);

                    if ( !world_to_local_success ) {
                        return;
                    }

                    touchable_under_mouse& under_mouse_c
                        = e.ensure_component<touchable_under_mouse>();

                    under_mouse_c.world_point = v2f(world_point);
                    under_mouse_c.local_point = v2f(v4f(world_point, 1.f) * world_to_local);
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
