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

    void update_slider_inputs(ecs::registry& owner) {
        owner.for_joined_components<slider, widget, events<touchable_events::event>, actor>([](
            const ecs::const_entity&,
            slider& s,
            widget& w,
            const events<touchable_events::event>& events,
            const actor& a)
        {
            for ( const touchable_events::event& evt : events.get() ) {
                if ( auto mouse_evt = std::get_if<touchable_events::mouse_drag_evt>(&evt);
                    mouse_evt &&
                    a.node() &&
                    w.size().x > 0.f &&
                    w.size().y > 0.f )
                {
                    const v2f local_point = v2f(a.node()->world_to_local(
                        v4f(mouse_evt->world_point(), 0.f, 1.f)));

                    switch ( s.direction() ) {
                    case slider::directions::row:
                        s.normalized_value(local_point.x / w.size().x);
                        break;
                    case slider::directions::row_reversed:
                        s.normalized_value(1.f - local_point.x / w.size().x);
                        break;
                    case slider::directions::column:
                        s.normalized_value(local_point.y / w.size().y);
                        break;
                    case slider::directions::column_reversed:
                        s.normalized_value(1.f - local_point.y / w.size().y);
                        break;
                    default:
                        E2D_ASSERT_MSG(false, "unexpected slider direction type");
                        break;
                    }

                }

                if ( auto mouse_evt = std::get_if<touchable_events::mouse_scroll_evt>(&evt);
                    mouse_evt &&
                    !math::is_near_zero(mouse_evt->delta().y) )
                {
                    const f32 slider_range = s.max_value() - s.min_value();
                    const f32 delta_step = slider_range * 0.01f * mouse_evt->delta().y;

                    switch ( s.direction() ) {
                    case slider::directions::row:
                        s.value(s.raw_value() + delta_step);
                        break;
                    case slider::directions::row_reversed:
                        s.value(s.raw_value() - delta_step);
                        break;
                    case slider::directions::column:
                        s.value(s.raw_value() + delta_step);
                        break;
                    case slider::directions::column_reversed:
                        s.value(s.raw_value() - delta_step);
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
            gcomponent<slider_handle> slider_h =
                nodes::find_component_from_children<slider_handle>(
                    a.node(),
                    nodes::options().recursive(true));

            if ( !slider_h ) {
                return;
            }

            gcomponent<actor> slider_h_a = slider_h.owner().component<actor>();
            if ( !slider_h_a || !slider_h_a->node() ) {
                return;
            }

            gcomponent<widget> slider_h_area_w = slider_h_a->node()->parent()
                ? slider_h_a->node()->parent()->owner().component<widget>()
                : gcomponent<widget>();

            if ( !slider_h_area_w ) {
                return;
            }

            switch ( s.direction() ) {
            case slider::directions::row:
                slider_h_a->node()->translation(
                    v2f::unit_x() *
                    slider_h_area_w->size().x *
                    s.normalized_value());
                break;
            case slider::directions::row_reversed:
                slider_h_a->node()->translation(
                    v2f::unit_x() *
                    slider_h_area_w->size().x *
                    (1.f - s.normalized_value()));
                break;
            case slider::directions::column:
                slider_h_a->node()->translation(
                    v2f::unit_y() *
                    slider_h_area_w->size().y *
                    s.normalized_value());
                break;
            case slider::directions::column_reversed:
                slider_h_a->node()->translation(
                    v2f::unit_y() *
                    slider_h_area_w->size().y *
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
