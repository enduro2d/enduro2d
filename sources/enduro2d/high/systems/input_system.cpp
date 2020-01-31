/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/input_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/colliders.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scene.hpp>
#include <enduro2d/high/components/touchable.hpp>

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/node.hpp>

#define PNPOLY_IMPLEMENTATION
#include <3rdparty/pnpoly.h/pnpoly.h>

namespace
{
    using namespace e2d;

    struct world_space_rect_collider {
        using src_collider_t = rect_collider;
        std::array<v3f, 4> points{};
    };

    struct world_space_circle_collider {
        using src_collider_t = circle_collider;
        std::array<v3f, 12> points{};
    };

    struct world_space_polygon_collider {
        using src_collider_t = polygon_collider;
        vector<v3f> points{};
    };

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

    template < typename WorldSpaceCollider >
    void update_world_space_colliders(ecs::registry& owner) {
        using dst_collider_t = WorldSpaceCollider;
        using src_collider_t = typename WorldSpaceCollider::src_collider_t;
        owner.for_joined_components<touchable, src_collider_t, actor>([](
            ecs::entity e,
            const touchable&,
            const src_collider_t& src,
            const actor& a)
        {
            update_world_space_collider(
                e.ensure_component<dst_collider_t>(),
                src,
                a.node() ? a.node()->world_matrix() : m4f::identity());
        }, !ecs::exists_any<disabled<touchable>, disabled<src_collider_t>>());
    }
}

namespace
{
    using namespace e2d;

    bool is_collider_under_mouse(
        const world_space_rect_collider& c,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport) noexcept
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

    bool is_collider_under_mouse(
        const world_space_circle_collider& c,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport) noexcept
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

    bool is_collider_under_mouse(
        const world_space_polygon_collider& c,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport) noexcept
    {
        static thread_local std::vector<v2f> points;
        points.clear();

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

    template < typename WorldSpaceCollider >
    void update_colliders_under_mouse(
        ecs::registry& owner,
        const v2f& mouse_p,
        const m4f& camera_vp,
        const b2f& camera_viewport)
    {
        using world_space_collider_t = WorldSpaceCollider;
        owner.for_joined_components<touchable, world_space_collider_t>([
            &mouse_p,
            &camera_vp,
            &camera_viewport
        ](ecs::entity e, const touchable&, const world_space_collider_t& c){
            if ( is_collider_under_mouse(c, mouse_p, camera_vp, camera_viewport) ) {
                e.ensure_component<touchable::under_mouse>();
            }
        }, !ecs::exists_any<disabled<touchable>, disabled<world_space_collider_t>>());
    }
}

namespace e2d
{
    //
    // input_system::internal_state
    //

    class input_system::internal_state final : private noncopyable {
    public:
        internal_state(input& i, window& w)
        : input_(i)
        , window_(w) {}
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            owner.remove_all_components<touchable::touched>();
            owner.remove_all_components<touchable::under_mouse>();

            update_world_space_colliders<world_space_rect_collider>(owner);
            update_world_space_colliders<world_space_circle_collider>(owner);
            update_world_space_colliders<world_space_polygon_collider>(owner);

            owner.for_joined_components<camera::input, camera>([this, &owner](
                const ecs::const_entity&,
                const camera::input&,
                const camera& camera)
            {
                const v2u& target_size = camera.target()
                    ? camera.target()->size()
                    : window_.framebuffer_size();

                const v2f& mouse_p = math::normalized_to_point(
                    b2f(target_size.cast_to<f32>()),
                    math::point_to_normalized(
                        b2f(window_.real_size().cast_to<f32>()),
                        input_.mouse().cursor_pos()));

                const m4f& camera_vp =
                    camera.view() * camera.projection();

                const b2f& camera_viewport = b2f(
                    camera.viewport().position * target_size.cast_to<f32>(),
                    camera.viewport().size * target_size.cast_to<f32>());

                if ( !math::inside(camera_viewport, mouse_p) ) {
                    return;
                }

                update_colliders_under_mouse<world_space_rect_collider>(
                    owner,
                    mouse_p,
                    camera_vp,
                    camera_viewport);

                update_colliders_under_mouse<world_space_circle_collider>(
                    owner,
                    mouse_p,
                    camera_vp,
                    camera_viewport);

                update_colliders_under_mouse<world_space_polygon_collider>(
                    owner,
                    mouse_p,
                    camera_vp,
                    camera_viewport);
            }, !ecs::exists<disabled<camera>>());
        }
    private:
        input& input_;
        window& window_;
    };

    //
    // input_system
    //

    input_system::input_system()
    : state_(new internal_state(the<input>(), the<window>())) {}
    input_system::~input_system() noexcept = default;

    void input_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("input_system.process_update");
        state_->process_update(owner);
    }
}
