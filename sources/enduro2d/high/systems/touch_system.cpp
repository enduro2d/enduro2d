/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/touch_system.hpp>

#include "touch_system_impl/touch_system_base.hpp"
#include "touch_system_impl/touch_system_colliders.hpp"
#include "touch_system_impl/touch_system_dispatcher.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;
}

namespace e2d
{
    //
    // touch_system::internal_state
    //

    class touch_system::internal_state final : private noncopyable {
    public:
        internal_state(input& i, window& w)
        : input_(i)
        , window_(w)
        , dispatcher_(window_.register_event_listener<dispatcher>()) {}

        ~internal_state() noexcept {
            window_.unregister_event_listener(dispatcher_);
        }

        void process_update(ecs::registry& owner) {
            update_world_space_colliders(owner);
            update_world_space_colliders_under_mouse(input_, window_, owner);
            dispatcher_.dispatch_all_events(owner);
        }
    private:
        input& input_;
        window& window_;
        dispatcher& dispatcher_;
    };

    //
    // touch_system
    //

    touch_system::touch_system()
    : state_(new internal_state(the<input>(), the<window>())) {}
    touch_system::~touch_system() noexcept = default;

    void touch_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_update(owner);
    }
}
