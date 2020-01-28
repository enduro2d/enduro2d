/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/camera_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/camera.hpp>

namespace
{
    using namespace e2d;

    m4f make_camera_view(const actor& actor) noexcept {
        return actor.node()
            ? math::inversed(actor.node()->world_matrix()).first
            : m4f::identity();
    }

    m4f make_camera_projection(const camera& camera, const window& window) noexcept {
        const f32 ortho_znear = camera.znear();
        const f32 ortho_zfar = math::max(
            camera.zfar(),
            camera.znear() + math::default_precision<f32>());

        const v2f target_size = camera.target()
            ? camera.target()->size().cast_to<f32>()
            : window.real_size().cast_to<f32>();
        const v2f virtual_size = window.virtual_size().cast_to<f32>();

        if ( math::is_near_zero(math::length_squared(target_size), 0.f) ||
            math::is_near_zero(math::length_squared(virtual_size), 0.f) )
        {
            return camera.projection();
        }

        const f32 target_aspect = target_size.x / target_size.y;
        const f32 virtual_aspect = virtual_size.x / virtual_size.y;

        switch ( camera.mode() ) {
        case camera::modes::manual:
            return camera.projection();
        case camera::modes::stretch:
            return math::make_orthographic_lh_matrix4(
                virtual_size,
                ortho_znear,
                ortho_zfar);
        case camera::modes::flexible:
            return math::make_orthographic_lh_matrix4(
                target_size,
                ortho_znear,
                ortho_zfar);
        case camera::modes::fixed_fit:
            return math::make_orthographic_lh_matrix4(
                target_aspect < virtual_aspect
                    ? v2f(virtual_size.x, virtual_size.y * (virtual_aspect / target_aspect))
                    : v2f(virtual_size.x * (target_aspect / virtual_aspect), virtual_size.y),
                ortho_znear,
                ortho_zfar);
        case camera::modes::fixed_crop:
            return math::make_orthographic_lh_matrix4(
                virtual_aspect < target_aspect
                    ? v2f(virtual_size.x, virtual_size.y * (virtual_aspect / target_aspect))
                    : v2f(virtual_size.x * (target_aspect / virtual_aspect), virtual_size.y),
                ortho_znear,
                ortho_zfar);
        default:
            E2D_ASSERT_MSG(false, "unexpected camera mode");
            return camera.projection();
        }
    }
}

namespace e2d
{
    //
    // camera_system::internal_state
    //

    class camera_system::internal_state final : private noncopyable {
    public:
        internal_state(window& w)
        : window_(w) {}
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            owner.for_joined_components<camera, actor>([](
                const ecs::const_entity&,
                camera& c,
                const actor& a)
            {
                c.view(make_camera_view(a));
            });

            owner.for_each_component<camera>([this](
                const ecs::const_entity&,
                camera& c)
            {
                c.projection(make_camera_projection(c, window_));
            });
        }
    private:
        window& window_;
    };

    //
    // camera_system
    //

    camera_system::camera_system()
    : state_(new internal_state(the<window>())) {}
    camera_system::~camera_system() noexcept = default;

    void camera_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("camera_system.process_update");
        state_->process_update(owner);
    }
}
