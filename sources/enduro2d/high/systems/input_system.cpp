/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/input_system.hpp>

#include <enduro2d/high/components/colliders.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    //
    // input_system::internal_state
    //

    class input_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process(ecs::registry& owner) {
        }
    };

    //
    // input_system
    //

    input_system::input_system()
    : state_(new internal_state()) {}
    input_system::~input_system() noexcept = default;

    void input_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process(owner);
    }
}