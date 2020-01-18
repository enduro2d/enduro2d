/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/editor_system.hpp>

namespace e2d
{
    //
    // editor_system::internal_state
    //

    class editor_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process(ecs::registry& owner) {
            E2D_UNUSED(owner);
        }
    };

    //
    // editor_system
    //

    editor_system::editor_system()
    : state_(new internal_state()) {}
    editor_system::~editor_system() noexcept = default;

    void editor_system::process(
        ecs::registry& owner,
        const ecs::after<systems::post_render_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("editor_system.process");
        state_->process(owner);
    }
}
