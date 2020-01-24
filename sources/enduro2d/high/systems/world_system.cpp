/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/world_system.hpp>

#include <enduro2d/high/world.hpp>

namespace e2d
{
    //
    // world_system::internal_state
    //

    class world_system::internal_state final : private noncopyable {
    public:
        internal_state(world& w)
        : world_(w) {}
        ~internal_state() noexcept = default;

        void process_frame_finalize(ecs::registry& owner) {
            E2D_UNUSED(owner);
            world_.finalize_instances();
        }
    private:
        world& world_;
    };

    //
    // world_system
    //

    world_system::world_system()
    : state_(new internal_state(the<world>())) {}
    world_system::~world_system() noexcept = default;

    void world_system::process(
        ecs::registry& owner,
        const ecs::after<systems::frame_finalize_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("world_system.process_frame_finalize");
        state_->process_frame_finalize(owner);
    }
}
