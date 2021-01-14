/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/progress_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/progress.hpp>
#include <enduro2d/high/components/progress_handle.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    void update_progress_states(ecs::registry& owner) {
        owner.for_joined_components<progress, actor>([](
            const ecs::const_entity&,
            const progress& s,
            const actor& a)
        {
            gcomponent<progress_handle> handle_h =
                nodes::find_component_from_children<progress_handle>(
                    a.node(),
                    nodes::options().recursive(true));

            if ( !handle_h ) {
                return;
            }

            gcomponent<actor> handle_a = handle_h.component<actor>();
            if ( !handle_a || !handle_a->node() ) {
                return;
            }

            gcomponent<widget> handle_w = handle_h.component<widget>();
            if ( !handle_w ) {
                return;
            }

            gcomponent<widget> handle_area_w = handle_a->node()->parent()
                ? handle_a->node()->parent()->owner().component<widget>()
                : gcomponent<widget>();

            if ( !handle_area_w ) {
                return;
            }

            const v2f& area_size = handle_area_w->size();

            switch ( s.direction() ) {
            case progress::directions::row:
                handle_a->node()->transform({
                    v2f::zero(),
                    0.f,
                    v2f::unit()});
                widgets::change_size(handle_w, {
                    s.value() * area_size.x,
                    area_size.y});
                break;
            case progress::directions::row_reversed:
                handle_a->node()->transform({
                    v2f::unit_x() * (1.f - s.value()) * area_size.x,
                    0.f,
                    v2f::unit()});
                widgets::change_size(handle_w, {
                    s.value() * area_size.x,
                    area_size.y});
                break;
            case progress::directions::column:
                handle_a->node()->transform({
                    v2f::zero(),
                    0.f,
                    v2f::unit()});
                widgets::change_size(handle_w, {
                    area_size.x,
                    s.value() * area_size.y});
                break;
            case progress::directions::column_reversed:
                handle_a->node()->transform({
                    v2f::unit_y() * (1.f - s.value()) * area_size.y,
                    0.f,
                    v2f::unit()});
                widgets::change_size(handle_w, {
                    area_size.x,
                    s.value() * area_size.y});
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected progress direction type");
                break;
            }
        }, !ecs::exists<disabled<progress>>());
    }
}

namespace e2d
{
    //
    // progress_system::internal_state
    //

    class progress_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            update_progress_states(owner);
        }
    };

    //
    // progress_system
    //

    progress_system::progress_system()
    : state_(new internal_state()) {}
    progress_system::~progress_system() noexcept = default;

    void progress_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_update(owner);
    }
}
