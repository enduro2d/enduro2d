/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/systems/sprite_system.hpp>

#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

namespace
{
    using namespace e2d;

    const str_hash texture_sampler_hash = "u_texture";

    void update_texture_sampler(const sprite_renderer& s, renderer& r) {
        const texture_ptr texture = s.sprite() && s.sprite()->content().texture()
            ? s.sprite()->content().texture()->content()
            : nullptr;
        const render::sampler_min_filter min_filter = s.filtering()
            ? render::sampler_min_filter::linear
            : render::sampler_min_filter::nearest;
        const render::sampler_mag_filter mag_filter = s.filtering()
            ? render::sampler_mag_filter::linear
            : render::sampler_mag_filter::nearest;
        render::sampler_state* const ts_p = r.properties().sampler(texture_sampler_hash);
        if ( ts_p ) {
            ts_p->texture(texture);
            ts_p->min_filter(min_filter);
            ts_p->mag_filter(mag_filter);
        } else {
            r.properties().sampler(texture_sampler_hash, render::sampler_state()
                .texture(texture)
                .min_filter(min_filter)
                .mag_filter(mag_filter));
        }
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
    };

    //
    // sprite_system
    //

    sprite_system::sprite_system()
    : state_(new internal_state) {}
    sprite_system::~sprite_system() noexcept = default;

    void sprite_system::process(ecs::registry& owner) {
        owner.for_joined_components<sprite_renderer, renderer>(
            [](const ecs::const_entity&, const sprite_renderer& s, renderer& r){
                update_texture_sampler(s, r);
            });
    }
}
