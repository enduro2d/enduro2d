/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/toggle_system.hpp>

#include <enduro2d/high/components/toggle.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/touchable.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

namespace
{
    using namespace e2d;

    void update_toggle_states(ecs::registry& owner) {
        owner.for_joined_components<toggle, touchable::clicked>([](
            ecs::entity e,
            const toggle&,
            const touchable::clicked&)
        {
            if ( e.exists_component<toggle::pressed>() ) {
                e.remove_component<toggle::pressed>();
            } else {
                e.ensure_component<toggle::pressed>();
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
