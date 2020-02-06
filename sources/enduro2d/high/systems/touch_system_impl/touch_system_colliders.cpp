/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "touch_system_colliders.hpp"

#define PNPOLY_IMPLEMENTATION
#include <3rdparty/pnpoly.h/pnpoly.h>

namespace e2d::touch_system_impl::impl
{
    void update_world_space_collider(
        world_space_rect_collider& dst,
        const rect_collider& src,
        const m4f& local_to_world)
    {
        const v2f& of = src.offset();
        const v2f& hs = src.size() * 0.5f;

        const v2f p1{of.x - hs.x, of.y - hs.y};
        const v2f p2{of.x + hs.x, of.y - hs.y};
        const v2f p3{of.x + hs.x, of.y + hs.y};
        const v2f p4{of.x - hs.x, of.y + hs.y};

        dst.points[0] = v3f(v4f(p1, 0.f, 1.f) * local_to_world);
        dst.points[1] = v3f(v4f(p2, 0.f, 1.f) * local_to_world);
        dst.points[2] = v3f(v4f(p3, 0.f, 1.f) * local_to_world);
        dst.points[3] = v3f(v4f(p4, 0.f, 1.f) * local_to_world);
    }

    void update_world_space_collider(
        world_space_circle_collider& dst,
        const circle_collider& src,
        const m4f& local_to_world)
    {
        const v2f& of = src.offset();
        for ( std::size_t i = 0, e = dst.points.size(); i < e; ++i ) {
            const radf a =
                math::two_pi<f32>() /
                math::numeric_cast<f32>(e) *
                math::numeric_cast<f32>(i);
            const v2f p =
                of +
                v2f(math::cos(a), math::sin(a)) *
                src.radius();
            dst.points[i] = v3f(v4f(p, 0.f, 1.f) * local_to_world);
        }
    }

    void update_world_space_collider(
        world_space_polygon_collider& dst,
        const polygon_collider& src,
        const m4f& local_to_world)
    {
        const vector<v2f>& src_points = src.points();

        dst.points.clear();
        if ( dst.points.capacity() < src_points.size() ) {
            dst.points.reserve(math::max(dst.points.capacity() * 2u, src_points.size()));
        }

        const v2f& of = src.offset();
        for ( std::size_t i = 0, e = src_points.size(); i < e; ++i ) {
            const v2f p = of + src_points[i];
            dst.points.push_back(v3f(v4f(p, 0.f, 1.f) * local_to_world));
        }
    }
}

namespace e2d::touch_system_impl::impl
{
    bool is_world_space_collider_under_mouse(
        const world_space_rect_collider& c,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport)
    {
        std::array<
            v2f,
            std::tuple_size_v<decltype(c.points)>
        > points;

        std::transform(c.points.begin(), c.points.end(), points.begin(), [
            &camera_vp,
            &camera_viewport
        ](const v3f& point) noexcept {
            return v2f(math::project(point, camera_vp, camera_viewport).first);
        });

        return !!pnpoly_aos(
            math::numeric_cast<int>(points.size()),
            points.data()->data(),
            mouse_p.x, mouse_p.y);
    }

    bool is_world_space_collider_under_mouse(
        const world_space_circle_collider& c,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport)
    {
        std::array<
            v2f,
            std::tuple_size_v<decltype(c.points)>
        > points;

        std::transform(c.points.begin(), c.points.end(), points.begin(), [
            &camera_vp,
            &camera_viewport
        ](const v3f& point) noexcept {
            return v2f(math::project(point, camera_vp, camera_viewport).first);
        });

        return !!pnpoly_aos(
            math::numeric_cast<int>(points.size()),
            points.data()->data(),
            mouse_p.x, mouse_p.y);
    }

    bool is_world_space_collider_under_mouse(
        const world_space_polygon_collider& c,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport)
    {
        static thread_local std::vector<v2f> points;
        E2D_DEFER([](){ points.clear(); });

        if ( points.capacity() < c.points.size() ) {
            points.reserve(math::max(points.capacity() * 2u, c.points.size()));
        }

        std::transform(c.points.begin(), c.points.end(), std::back_inserter(points), [
            &camera_vp,
            &camera_viewport
        ](const v3f& point) noexcept {
            return v2f(math::project(point, camera_vp, camera_viewport).first);
        });

        return !!pnpoly_aos(
            math::numeric_cast<int>(points.size()),
            points.data()->data(),
            mouse_p.x, mouse_p.y);
    }
}

namespace e2d::touch_system_impl
{
    void update_world_space_colliders(ecs::registry& owner) {
        impl::update_world_space_colliders<world_space_rect_collider>(owner);
        impl::update_world_space_colliders<world_space_circle_collider>(owner);
        impl::update_world_space_colliders<world_space_polygon_collider>(owner);
    }

    void update_world_space_colliders_under_mouse(input& input, window& window, ecs::registry& owner) {
        owner.remove_all_components<touchable_under_mouse>();
        owner.for_joined_components<camera::input, camera>([&input, &window, &owner](
            const ecs::const_entity&,
            const camera::input&,
            const camera& camera)
        {
            const v2u& target_size = camera.target()
                ? camera.target()->size()
                : window.framebuffer_size();

            const v2f& mouse_p = math::normalized_to_point(
                b2f(target_size.cast_to<f32>()),
                math::point_to_normalized(
                    b2f(window.real_size().cast_to<f32>()),
                    input.mouse().cursor_pos()));

            const m4f& camera_vp =
                camera.view() * camera.projection();

            const b2f& camera_viewport = b2f(
                camera.viewport().position * target_size.cast_to<f32>(),
                camera.viewport().size * target_size.cast_to<f32>());

            if ( !math::inside(camera_viewport, mouse_p) ) {
                return;
            }

            impl::update_world_space_colliders_under_mouse<world_space_rect_collider>(
                owner,
                mouse_p,
                camera_vp,
                camera_viewport);

            impl::update_world_space_colliders_under_mouse<world_space_circle_collider>(
                owner,
                mouse_p,
                camera_vp,
                camera_viewport);

            impl::update_world_space_colliders_under_mouse<world_space_polygon_collider>(
                owner,
                mouse_p,
                camera_vp,
                camera_viewport);
        }, !ecs::exists_any<
            disabled<actor>,
            disabled<camera>>());
    }
}
