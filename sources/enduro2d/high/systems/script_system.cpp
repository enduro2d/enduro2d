/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/script_system.hpp>

namespace e2d
{
    //
    // script_system::internal_state
    //

    class script_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void update_process(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }

        void finalize_process(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // script_system
    //

    script_system::script_system()
    : state_(new internal_state()) {}
    script_system::~script_system() noexcept = default;

    void script_system::process(
        ecs::registry& owner,
        const systems::update_event& event)
    {
        E2D_UNUSED(event);
        state_->update_process(owner);
    }

    void script_system::process(
        ecs::registry& owner,
        const ecs::after<systems::frame_finalize_event>& event)
    {
        E2D_UNUSED(event);
        state_->finalize_process(owner);
    }
}
