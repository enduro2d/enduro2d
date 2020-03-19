/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/widget_system.hpp>

namespace e2d
{
    //
    // widget_system::internal_state
    //

    class widget_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // widget_system
    //

    widget_system::widget_system()
    : state_(new internal_state()) {}
    widget_system::~widget_system() noexcept = default;

    void widget_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("widget_system.process_update");
        state_->process_update(owner);
    }
}
