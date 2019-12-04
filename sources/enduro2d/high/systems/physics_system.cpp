/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/physics_system.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    //
    // physics_system::internal_state
    //

    class physics_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process(f32 dt, ecs::registry& owner) {
        }
    };

    //
    // physics_system
    //

    physics_system::physics_system()
    : state_(new internal_state()) {}
    physics_system::~physics_system() noexcept = default;

    void physics_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        state_->process(trigger.event.dt, owner);
    }
}
