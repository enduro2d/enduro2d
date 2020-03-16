/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/slider_system.hpp>

namespace
{
    using namespace e2d;

    void update_slider_states(ecs::registry& owner) {
        E2D_UNUSED(owner);
    }

    void update_slider_styles(ecs::registry& owner) {
        E2D_UNUSED(owner);
    }
}

namespace e2d
{
    //
    // slider_system::internal_state
    //

    class slider_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_states(ecs::registry& owner) {
            update_slider_states(owner);
        }

        void process_styles(ecs::registry& owner) {
            update_slider_styles(owner);
        }
    };

    //
    // slider_system
    //

    slider_system::slider_system()
    : state_(new internal_state()) {}
    slider_system::~slider_system() noexcept = default;

    void slider_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("slider_system.process_states");
        state_->process_states(owner);
    }

    void slider_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("slider_system.process_styles");
        state_->process_styles(owner);
    }
}
