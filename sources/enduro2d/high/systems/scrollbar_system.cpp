/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/scrollbar_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/events.hpp>
#include <enduro2d/high/components/scrollbar.hpp>
#include <enduro2d/high/components/scrollbar_handle.hpp>
#include <enduro2d/high/components/touchable.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    struct scrollbar_drag_offset {
        v2f local_offset{v2f::zero()};
    };

    void update_scrollbar_inputs(ecs::registry& owner) {
        owner.for_joined_components<scrollbar, events<touchable_events::event>, actor>([](
            const ecs::const_entity&,
            scrollbar& s,
            const events<touchable_events::event>& events,
            const actor& a)
        {
            if ( events.empty() ) {
                return;
            }

            gcomponent<scrollbar_handle> handle_h =
                nodes::find_component_from_children<scrollbar_handle>(
                    a.node(),
                    nodes::options().recursive(true));

            if ( !handle_h ) {
                return;
            }

            gcomponent<actor> handle_a = handle_h.component<actor>();
            if ( !handle_a || !handle_a->node() ) {
                return;
            }

            gcomponent<widget> handle_area_w = handle_a->node()->parent()
                ? handle_a->node()->parent()->owner().component<widget>()
                : gcomponent<widget>();

            if ( !handle_area_w ) {
                return;
            }

            gcomponent<actor> handle_area_a = handle_area_w.component<actor>();
            if ( !handle_area_a || !handle_area_a->node() ) {
                return;
            }

            for ( const touchable_events::event& evt : events.get() ) {

                //
                // mouse_drag_evt
                //

                if ( auto mouse_evt = std::get_if<touchable_events::mouse_drag_evt>(&evt) ) {

                    if ( mouse_evt->type() == touchable_events::mouse_drag_evt::types::start ) {
                        const gobject target = mouse_evt->target();
                        const const_gcomponent<actor> target_a{target};
                        const const_node_iptr target_n = target_a ? target_a->node() : const_node_iptr();

                        const v2f local_offset = target_n && target_n->has_parent_recursive(handle_a->node())
                            ? v2f(handle_a->node()->world_to_local(v4f(mouse_evt->world_point(), 0.f, 1.f)))
                            : v2f::zero();

                        handle_a.component<scrollbar_drag_offset>().ensure().local_offset = local_offset;
                    }

                    const v2f local_point = v2f(handle_area_a->node()->world_to_local(
                        v4f(mouse_evt->world_point(), 0.f, 1.f)));

                    const v2f local_offset = handle_a.component<scrollbar_drag_offset>()
                        ? handle_a.component<scrollbar_drag_offset>()->local_offset
                        : v2f::zero();

                    switch ( s.direction() ) {
                    case scrollbar::directions::row:
                        if ( const f32 sw = handle_area_w->size().x; sw > 0.f ) {
                            s.value((local_point.x - local_offset.x) / sw);
                        }
                        break;
                    case scrollbar::directions::row_reversed:
                        if ( const f32 sw = handle_area_w->size().x; sw > 0.f ) {
                            s.value(1.f - (local_point.x - local_offset.x) / sw);
                        }
                        break;
                    case scrollbar::directions::column:
                        if ( const f32 sh = handle_area_w->size().y; sh > 0.f ) {
                            s.value((local_point.y - local_offset.y) / sh);
                        }
                        break;
                    case scrollbar::directions::column_reversed:
                        if ( const f32 sh = handle_area_w->size().y; sh > 0.f ) {
                            s.value(1.f - (local_point.y - local_offset.y) / sh);
                        }
                        break;
                    default:
                        E2D_ASSERT_MSG(false, "unexpected scrollbar direction type");
                        break;
                    }

                }

                //
                // mouse_scroll_evt
                //

                if ( auto mouse_evt = std::get_if<touchable_events::mouse_scroll_evt>(&evt);
                    mouse_evt &&
                    !math::is_near_zero(mouse_evt->delta().y) )
                {
                    const f32 delta_step = 0.01f * mouse_evt->delta().y;

                    switch ( s.direction() ) {
                    case scrollbar::directions::row:
                        s.value(s.value() + delta_step);
                        break;
                    case scrollbar::directions::row_reversed:
                        s.value(s.value() - delta_step);
                        break;
                    case scrollbar::directions::column:
                        s.value(s.value() + delta_step);
                        break;
                    case scrollbar::directions::column_reversed:
                        s.value(s.value() - delta_step);
                        break;
                    default:
                        E2D_ASSERT_MSG(false, "unexpected scrollbar direction type");
                        break;
                    }
                }
            }
        }, !ecs::exists<disabled<scrollbar>>());
    }

    void update_scrollbar_states(ecs::registry& owner) {
        owner.for_joined_components<scrollbar, actor>([](
            const ecs::const_entity&,
            const scrollbar& s,
            const actor& a)
        {
            gcomponent<scrollbar_handle> handle_h =
                nodes::find_component_from_children<scrollbar_handle>(
                    a.node(),
                    nodes::options().recursive(true));

            if ( !handle_h ) {
                return;
            }

            gcomponent<actor> handle_a = handle_h.component<actor>();
            if ( !handle_a || !handle_a->node() ) {
                return;
            }

            gcomponent<widget> handle_area_w = handle_a->node()->parent()
                ? handle_a->node()->parent()->owner().component<widget>()
                : gcomponent<widget>();

            if ( !handle_area_w ) {
                return;
            }

            switch ( s.direction() ) {
            case scrollbar::directions::row:
                handle_a->node()->translation(
                    v2f::unit_x() *
                    handle_area_w->size().x *
                    s.value());
                break;
            case scrollbar::directions::row_reversed:
                handle_a->node()->translation(
                    v2f::unit_x() *
                    handle_area_w->size().x *
                    (1.f - s.value()));
                break;
            case scrollbar::directions::column:
                handle_a->node()->translation(
                    v2f::unit_y() *
                    handle_area_w->size().y *
                    s.value());
                break;
            case scrollbar::directions::column_reversed:
                handle_a->node()->translation(
                    v2f::unit_y() *
                    handle_area_w->size().y *
                    (1.f - s.value()));
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected scrollbar direction type");
                break;
            }
        }, !ecs::exists<disabled<scrollbar>>());
    }
}

namespace e2d
{
    //
    // scrollbar_system::internal_state
    //

    class scrollbar_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            update_scrollbar_inputs(owner);
            update_scrollbar_states(owner);
        }
    };

    //
    // scrollbar_system
    //

    scrollbar_system::scrollbar_system()
    : state_(new internal_state()) {}
    scrollbar_system::~scrollbar_system() noexcept = default;

    void scrollbar_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_update(owner);
    }
}
