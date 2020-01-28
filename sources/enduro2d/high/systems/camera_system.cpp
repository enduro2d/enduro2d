/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/camera_system.hpp>

#include <enduro2d/high/components/camera.hpp>

namespace e2d
{
    //
    // camera_system::internal_state
    //

    class camera_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // camera_system
    //

    camera_system::camera_system()
    : state_(new internal_state()) {}
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
