/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/widget_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/layout.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    void update_dirty_widgets(ecs::registry& owner) {
        owner.for_joined_components<widget::dirty, widget, actor>([](
            const ecs::const_entity&,
            const widget::dirty&,
            const widget&,
            const actor& a)
        {
            if ( a.node() && a.node()->owner() ) {
                gcomponent<layout> l{a.node()->owner()};
                gcomponent<widget> w{a.node()->owner()};
                layouts::mark_dirty(l);
                layouts::mark_dirty(widgets::find_parent_layout(w));
            }
        }, !ecs::exists_any<
            disabled<actor>,
            disabled<widget>>());

        owner.remove_all_components<widget::dirty>();
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
            update_dirty_widgets(owner);
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
