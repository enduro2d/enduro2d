/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/slider_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/events.hpp>
#include <enduro2d/high/components/slider.hpp>
#include <enduro2d/high/components/slider_handle.hpp>
#include <enduro2d/high/components/touchable.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    struct slider_drag_offset {
        v2f local_offset{v2f::zero()};
    };

    void update_slider_inputs(ecs::registry& owner) {
        owner.for_joined_components<slider, events<touchable_events::event>, actor>([](
            const ecs::const_entity&,
            slider& s,
            const events<touchable_events::event>& events,
            const actor& a)
        {
            if ( events.empty() ) {
                return;
            }

            gcomponent<slider_handle> handle_h =
                nodes::find_component_from_children<slider_handle>(
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
                if ( auto mouse_evt = std::get_if<touchable_events::mouse_drag_evt>(&evt) ) {
                    if ( mouse_evt->type() == touchable_events::mouse_drag_evt::types::start ) {
                        const gobject target = mouse_evt->target();
                        const const_gcomponent<actor> target_a{target};
                        const const_node_iptr target_n = target_a ? target_a->node() : const_node_iptr();

                        const v2f local_offset = target_n && target_n->has_parent_recursive(handle_a->node())
                            ? v2f(handle_a->node()->world_to_local(v4f(mouse_evt->world_point(), 0.f, 1.f)))
                            : v2f::zero();

                        handle_a.component<slider_drag_offset>().ensure().local_offset = local_offset;
                    }

                    const v2f local_point = v2f(handle_area_a->node()->world_to_local(
                        v4f(mouse_evt->world_point(), 0.f, 1.f)));

                    const v2f local_offset = handle_a.component<slider_drag_offset>()
                        ? handle_a.component<slider_drag_offset>()->local_offset
                        : v2f::zero();

                    switch ( s.direction() ) {
                    case slider::directions::row:
                        if ( const f32 sw = handle_area_w->size().x; sw > 0.f ) {
                            s.normalized_value((local_point.x - local_offset.x) / sw);
                        }
                        break;
                    case slider::directions::row_reversed:
                        if ( const f32 sw = handle_area_w->size().x; sw > 0.f ) {
                            s.normalized_value(1.f - (local_point.x - local_offset.x) / sw);
                        }
                        break;
                    case slider::directions::column:
                        if ( const f32 sh = handle_area_w->size().y; sh > 0.f ) {
                            s.normalized_value((local_point.y - local_offset.y) / sh);
                        }
                        break;
                    case slider::directions::column_reversed:
                        if ( const f32 sh = handle_area_w->size().y; sh > 0.f ) {
                            s.normalized_value(1.f - (local_point.y - local_offset.y) / sh);
                        }
                        break;
                    default:
                        E2D_ASSERT_MSG(false, "unexpected slider direction type");
                        break;
                    }
                }
            }
        }, !ecs::exists<disabled<slider>>());
    }

    void update_slider_states(ecs::registry& owner) {
        owner.for_joined_components<slider, actor>([](
            const ecs::const_entity&,
            const slider& s,
            const actor& a)
        {
            gcomponent<slider_handle> handle_h =
                nodes::find_component_from_children<slider_handle>(
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
            case slider::directions::row:
                handle_a->node()->translation(
                    v2f::unit_x() *
                    handle_area_w->size().x *
                    s.normalized_value());
                break;
            case slider::directions::row_reversed:
                handle_a->node()->translation(
                    v2f::unit_x() *
                    handle_area_w->size().x *
                    (1.f - s.normalized_value()));
                break;
            case slider::directions::column:
                handle_a->node()->translation(
                    v2f::unit_y() *
                    handle_area_w->size().y *
                    s.normalized_value());
                break;
            case slider::directions::column_reversed:
                handle_a->node()->translation(
                    v2f::unit_y() *
                    handle_area_w->size().y *
                    (1.f - s.normalized_value()));
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected slider direction type");
                break;
            }
        }, !ecs::exists<disabled<slider>>());
    }
}

namespace e2d
{
    //
    // slider_system::internal_state
    //

    class slider_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            update_slider_inputs(owner);
            update_slider_states(owner);
        }
    };

    //
    // slider_system
    //

    slider_system::slider_system()
    : state_(new internal_state()) {}
    slider_system::~slider_system() noexcept = default;

    void slider_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("slider_system.process_update");
        state_->process_update(owner);
    }
}
