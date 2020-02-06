/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/layout_system.hpp>

#include <enduro2d/high/components/layout.hpp>

namespace e2d
{
    //
    // layout_system::internal_state
    //

    class layout_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // layout_system
    //

    layout_system::layout_system()
    : state_(new internal_state()) {}
    layout_system::~layout_system() noexcept = default;

    void layout_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("layout_system.process_update");
        state_->process_update(owner);
    }
}
