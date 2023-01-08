/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/widget_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    void update_was_dirty_flags(ecs::registry& owner) {
        RETURN_DEFER_HPP([&owner](){
            owner.remove_all_components<widget::dirty>();
        });

        owner.remove_all_components<widget::was_dirty>();

        owner.for_joined_components<widget::dirty>([
        ](ecs::entity e, const widget::dirty&) {
            e.ensure_component<widget::was_dirty>();
        });
    }
}

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
            update_was_dirty_flags(owner);
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
        state_->process_update(owner);
    }
}
