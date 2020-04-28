/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/slider_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/handle.hpp>
#include <enduro2d/high/components/slider.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    slider& slider_normalized_value(slider& s, f32 value) noexcept {
        return s.value(math::lerp(s.min_value(), s.max_value(), value));
    }

    [[nodiscard]] f32 slider_normalized_value(const slider& s) noexcept {
        return math::approximately(s.min_value(), s.max_value())
            ? 0.f
            : math::inverse_lerp(s.min_value(), s.max_value(), s.value());
    }
}

namespace
{
    using namespace e2d;

    void update_slider_states(ecs::registry& owner) {
        owner.for_joined_components<slider, actor>([](
            const ecs::const_entity&,
            const slider& s,
            const actor& a)
        {
            gcomponent<handle> slider_handle =
                nodes::find_component_from_children<handle>(
                    a.node(),
                    nodes::options().recursive(true));

            if ( !slider_handle ) {
                return;
            }

            gcomponent<actor> slider_handle_a = slider_handle.owner().component<actor>();
            if ( !slider_handle_a || !slider_handle_a->node() ) {
                return;
            }

            gcomponent<widget> slider_handle_area_w = slider_handle_a->node()->parent()
                ? slider_handle_a->node()->parent()->owner().component<widget>()
                : gcomponent<widget>();

            if ( !slider_handle_area_w ) {
                return;
            }

            switch ( s.direction() ) {
            case slider::directions::row:
                slider_handle_a->node()->translation(
                    v2f::unit_x() *
                    slider_handle_area_w->size().x *
                    slider_normalized_value(s));
                break;
            case slider::directions::row_reversed:
                slider_handle_a->node()->translation(
                    v2f::unit_x() *
                    slider_handle_area_w->size().x *
                    (1.f - slider_normalized_value(s)));
                break;
            case slider::directions::column:
                slider_handle_a->node()->translation(
                    v2f::unit_y() *
                    slider_handle_area_w->size().y *
                    slider_normalized_value(s));
                break;
            case slider::directions::column_reversed:
                slider_handle_a->node()->translation(
                    v2f::unit_y() *
                    slider_handle_area_w->size().y *
                    (1.f - slider_normalized_value(s)));
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected slider direction type");
                break;
            }
        }, !ecs::exists<disabled<slider>>());
    }

    void update_slider_styles(ecs::registry& owner) {
        E2D_UNUSED(owner);
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

        void process_states(ecs::registry& owner) {
            update_slider_states(owner);
        }

        void process_styles(ecs::registry& owner) {
            update_slider_styles(owner);
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
        E2D_PROFILER_SCOPE("slider_system.process_states");
        state_->process_states(owner);
    }

    void slider_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("slider_system.process_styles");
        state_->process_styles(owner);
    }
}
