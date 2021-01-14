/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/button_system.hpp>

#include <enduro2d/high/components/button.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/touchable.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

namespace
{
    using namespace e2d;

    void update_button_states(ecs::registry& owner) {
        owner.for_joined_components<button, touchable>([](
            ecs::entity e,
            const button&,
            const touchable&)
        {
            if ( e.exists_component<touchable::pushing>() ) {
                e.ensure_component<button::pressed>();
            } else {
                e.remove_component<button::pressed>();
            }
        }, !ecs::exists<disabled<button>>());
    }

    void update_button_styles(ecs::registry& owner) {
        owner.for_joined_components<button, touchable, sprite_renderer>([](
            const ecs::const_entity& e,
            const button& b,
            const touchable&,
            sprite_renderer& sr)
        {
            if ( !b.style() ) {
                return;
            }

            const button_style& style = b.style()->content();

            if ( e.exists_component<disabled<button>>() ) {
                sr.tint(style.disabled_tint());
                sr.sprite(style.disabled_sprite()
                    ? style.disabled_sprite()
                    : style.normal_sprite());
            } else if ( e.exists_component<touchable::pushing>() ) {
                sr.tint(style.pushing_tint());
                sr.sprite(style.pushing_sprite()
                    ? style.pushing_sprite()
                    : style.hovering_sprite()
                        ? style.hovering_sprite()
                        : style.normal_sprite());
            } else if ( e.exists_component<touchable::hovering>() ) {
                sr.tint(style.hovering_tint());
                sr.sprite(style.hovering_sprite()
                    ? style.hovering_sprite()
                    : style.normal_sprite());
            } else {
                sr.tint(style.normal_tint());
                sr.sprite(style.normal_sprite());
            }
        });
    }
}

namespace e2d
{
    //
    // button_system::internal_state
    //

    class button_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_states(ecs::registry& owner) {
            update_button_states(owner);
        }

        void process_styles(ecs::registry& owner) {
            update_button_styles(owner);
        }
    };

    //
    // button_system
    //

    button_system::button_system()
    : state_(new internal_state()) {}
    button_system::~button_system() noexcept = default;

    void button_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_states(owner);
    }

    void button_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_styles(owner);
    }
}
