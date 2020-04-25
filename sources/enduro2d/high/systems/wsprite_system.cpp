/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/wsprite_system.hpp>

#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>
#include <enduro2d/high/components/widget.hpp>
#include <enduro2d/high/components/wsprite.hpp>

namespace
{
    using namespace e2d;

    void update_sprite_renderer_sizes(ecs::registry& owner) {
        owner.for_joined_components<wsprite, widget, sprite_renderer>([](
            const ecs::const_entity&,
            const wsprite&,
            const widget& w,
            sprite_renderer& sr)
        {
            if ( !sr.sprite() ) {
                return;
            }

            const sprite& spr = sr.sprite()->content();
            const v2f& size = spr.outer_texrect().size;

            if ( size.x > 0.f && size.y > 0.f ) {
                sr.scale(w.size() / size);
            }
        }, !ecs::exists<disabled<wsprite>>());
    }
}

namespace e2d
{
    //
    // wsprite_system::internal_state
    //

    class wsprite_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            update_sprite_renderer_sizes(owner);
        }
    };

    //
    // wsprite_system
    //

    wsprite_system::wsprite_system()
    : state_(new internal_state()) {}
    wsprite_system::~wsprite_system() noexcept = default;

    void wsprite_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("wsprite_system.process_update");
        state_->process_update(owner);
    }
}
