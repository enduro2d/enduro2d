/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/sprite_system.hpp>

#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    void update_dirty_widget_sprite(const widget& w, sprite_renderer& sr) {
        if ( !sr.sprite() ) {
            return;
        }

        const sprite& spr = sr.sprite()->content();
        const v2f& size = spr.outer_texrect().size;

        if ( size.x > 0.f && size.y > 0.f ) {
            sr.scale(w.size() / size);
        }
    }

    void update_dirty_widget_sprites(ecs::registry& owner) {
        owner.for_joined_components<widget::was_dirty, widget, sprite_renderer>([](
            const ecs::const_entity&,
            const widget::was_dirty&,
            const widget& w,
            sprite_renderer& sr)
        {
            update_dirty_widget_sprite(w, sr);
        }, !ecs::exists_any<
            disabled<actor>,
            disabled<widget>>());
    }
}

namespace e2d
{
    //
    // sprite_system::internal_state
    //

    class sprite_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            update_dirty_widget_sprites(owner);
        }
    };

    //
    // sprite_system
    //

    sprite_system::sprite_system()
    : state_(new internal_state()) {}
    sprite_system::~sprite_system() noexcept = default;

    void sprite_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("sprite_system.process_update");
        state_->process_update(owner);
    }
}
