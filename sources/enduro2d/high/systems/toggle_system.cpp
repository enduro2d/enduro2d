/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/toggle_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>
#include <enduro2d/high/components/toggle_group.hpp>
#include <enduro2d/high/components/toggle.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace
{
    using namespace e2d;

    void check_allow_switch_off(
        const const_gcomponent<toggle_group>& group,
        ecs::entity last_unpressed)
    {
        if ( !group || group->allow_switch_off() ) {
            return;
        }

        const_gcomponent<actor> group_a = group.component<actor>();
        if ( !group_a || !group_a->node() ) {
            return;
        }

        const bool any_pressed = !!nodes::find_component_from_children<toggle::pressed>(
            group_a->node(),
            nodes::options().recursive(true));

        if ( !any_pressed ) {
            last_unpressed.ensure_component<toggle::pressed>();
        }
    }

    void check_allow_multiple_on(
        const const_gcomponent<toggle_group>& group,
        ecs::entity last_pressed)
    {
        if ( !group || group->allow_multiple_on() ) {
            return;
        }

        const_gcomponent<actor> group_a = group.component<actor>();
        if ( !group_a || !group_a->node() ) {
            return;
        }

        nodes::for_extracted_components_from_children<toggle::pressed>(
            group_a->node(),
            [&last_pressed](const const_gcomponent<toggle::pressed>& pressed){
                if ( pressed.owner().raw_entity() != last_pressed ) {
                    pressed.owner().component<toggle::pressed>().remove();
                }
            }, nodes::options().recursive(true));
    }

    void update_toggle_states(ecs::registry& owner) {
        owner.for_joined_components<toggle, touchable::clicked, actor>([](
            ecs::entity e,
            const toggle&,
            const touchable::clicked&,
            const actor& a)
        {
            const_gcomponent<toggle_group> group =
                nodes::find_component_from_parents<toggle_group>(
                    a.node(),
                    nodes::options()
                        .recursive(true));

            if ( e.exists_component<toggle::pressed>() ) {
                e.remove_component<toggle::pressed>();
                check_allow_switch_off(group, e);
            } else {
                e.ensure_component<toggle::pressed>();
                check_allow_multiple_on(group, e);
            }
        }, !ecs::exists<disabled<toggle>>());
    }

    void update_toggle_styles(ecs::registry& owner) {
        owner.for_joined_components<toggle, touchable, sprite_renderer>([](
            const ecs::const_entity& e,
            const toggle& b,
            const touchable&,
            sprite_renderer& sr)
        {
            if ( !b.style() ) {
                return;
            }

            const toggle_style& style = b.style()->content();

            if ( e.exists_component<disabled<toggle>>() ) {
                if ( e.exists_component<toggle::pressed>() ) {
                    sr.tint(style.pressed_disabled_tint());
                    sr.sprite(style.pressed_disabled_sprite()
                        ? style.pressed_disabled_sprite()
                        : style.pressed_sprite());
                } else {
                    sr.tint(style.normal_disabled_tint());
                    sr.sprite(style.normal_disabled_sprite()
                        ? style.normal_disabled_sprite()
                        : style.normal_sprite());
                }
            } else if ( e.exists_component<touchable::pushing>() ) {
                if ( e.exists_component<toggle::pressed>() ) {
                    sr.tint(style.pressed_pushing_tint());
                    sr.sprite(style.pressed_pushing_sprite()
                        ? style.pressed_pushing_sprite()
                        : style.pressed_sprite());
                } else {
                    sr.tint(style.normal_pushing_tint());
                    sr.sprite(style.normal_pushing_sprite()
                        ? style.normal_pushing_sprite()
                        : style.normal_sprite());
                }
            } else if ( e.exists_component<touchable::hovering>() ) {
                if ( e.exists_component<toggle::pressed>() ) {
                    sr.tint(style.pressed_hovering_tint());
                    sr.sprite(style.pressed_hovering_sprite()
                        ? style.pressed_hovering_sprite()
                        : style.pressed_sprite());
                } else {
                    sr.tint(style.normal_hovering_tint());
                    sr.sprite(style.normal_hovering_sprite()
                        ? style.normal_hovering_sprite()
                        : style.normal_sprite());
                }
            } else {
                if ( e.exists_component<toggle::pressed>() ) {
                    sr.tint(style.pressed_tint());
                    sr.sprite(style.pressed_sprite());
                } else {
                    sr.tint(style.normal_tint());
                    sr.sprite(style.normal_sprite());
                }
            }
        });
    }
}

namespace e2d
{
    //
    // toggle_system::internal_state
    //

    class toggle_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_states(ecs::registry& owner) {
            update_toggle_states(owner);
        }

        void process_styles(ecs::registry& owner) {
            update_toggle_styles(owner);
        }
    };

    //
    // toggle_system
    //

    toggle_system::toggle_system()
    : state_(new internal_state()) {}
    toggle_system::~toggle_system() noexcept = default;

    void toggle_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("toggle_system.process_states");
        state_->process_states(owner);
    }

    void toggle_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("toggle_system.process_styles");
        state_->process_styles(owner);
    }
}
