/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/scroll_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/scroll.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    //
    // scroll_system::internal_state
    //

    class scroll_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // scroll_system
    //

    scroll_system::scroll_system()
    : state_(new internal_state()) {}
    scroll_system::~scroll_system() noexcept = default;

    void scroll_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("scroll_system.process_update");
        state_->process_update(owner);
    }
}
