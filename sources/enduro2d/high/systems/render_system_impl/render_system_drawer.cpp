/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render_system_drawer.hpp"

#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

namespace
{
    using namespace e2d;

    const str_hash matrix_v_property_hash = "u_matrix_v";
    const str_hash matrix_p_property_hash = "u_matrix_p";
    const str_hash matrix_vp_property_hash = "u_matrix_vp";
    const str_hash game_time_property_hash = "u_game_time";
    const str_hash sprite_texture_sampler_hash = "u_texture";
}

namespace e2d { namespace render_system_impl
{
    //
    // drawer::context
    //

    drawer::context::context(
        const camera& cam,
        const const_node_iptr& cam_n,
        engine& engine,
        render& render,
        batcher_type& batcher)
    : batcher_(batcher)
    {
        const m4f& m_v = cam_n ? cam_n->world_matrix() : m4f::identity();
        const m4f& m_p = cam.projection();

        internal_properties_
            .property(matrix_v_property_hash, m_v)
            .property(matrix_p_property_hash, m_p)
            .property(matrix_vp_property_hash, m_v * m_p)
            .property(game_time_property_hash, engine.time());

        render.execute(render::command_block<3>()
            .add_command(render::target_command(cam.target()))
            .add_command(render::viewport_command(cam.viewport()))
            .add_command(render::clear_command()
                .color_value(cam.background())));
    }

    void drawer::context::draw(
        const const_node_iptr& node)
    {
        if ( !node || !node->entity().alive() ) {
            return;
        }

        ecs::const_entity node_e = node->entity();
        const renderer* node_r = node_e.find_component<renderer>();
        if ( node_r && node_r->enabled() ) {
            const model_renderer* mdl_r = node_e.find_component<model_renderer>();
            if ( mdl_r ) {
                draw(node, *node_r, *mdl_r);
            }
            const sprite_renderer* spr_r = node_e.find_component<sprite_renderer>();
            if ( spr_r ) {
                draw(node, *node_r, *spr_r);
            }
        }
    }

    void drawer::context::draw(
        const const_node_iptr& node,
        const renderer& node_r,
        const model_renderer& mdl_r)
    {
        //TODO(BlackMat): implme
        E2D_UNUSED(node, node_r, mdl_r);
    }

    void drawer::context::draw(
        const const_node_iptr& node,
        const renderer& node_r,
        const sprite_renderer& spr_r)
    {
        if ( !node || !node_r.enabled() ) {
            return;
        }

        if ( !spr_r.sprite() || !spr_r.sprite()->content().material()) {
            return;
        }

        const sprite& spr = spr_r.sprite()->content();

        const f32 sw = spr.size().x;
        const f32 sh = spr.size().y;

        const f32 hw = -sw * spr.pivot().x;
        const f32 hh = -sh * spr.pivot().y;

        const v4f p1{hw + 0.f, hh + 0.f, 0.f, 1.f};
        const v4f p2{hw + sw,  hh + 0.f, 0.f, 1.f};
        const v4f p3{hw + sw,  hh + sh,  0.f, 1.f};
        const v4f p4{hw + 0.f, hh + sh,  0.f, 1.f};

        const f32 tx = spr.texrect().position.x;
        const f32 ty = spr.texrect().position.y;
        const f32 tw = spr.texrect().size.x;
        const f32 th = spr.texrect().size.y;

        const m4f& sm = node->world_matrix();
        const color& tn = spr_r.tint();

        const batcher_type::index_type indices[] = {
            0u, 1u, 2u, 2u, 3u, 0u};

        const batcher_type::vertex_type vertices[] = {
            { v3f(p1 * sm), {tx + 0.f, ty + 0.f}, color32(tn) },
            { v3f(p2 * sm), {tx + tw,  ty + 0.f}, color32(tn) },
            { v3f(p3 * sm), {tx + tw,  ty + th }, color32(tn) },
            { v3f(p4 * sm), {tx + 0.f, ty + th }, color32(tn) }};

        const texture_ptr texture = spr.texture()
            ? spr.texture()->content()
            : nullptr;

        const render::sampler_min_filter min_filter = spr_r.filtering()
            ? render::sampler_min_filter::linear
            : render::sampler_min_filter::nearest;

        const render::sampler_mag_filter mag_filter = spr_r.filtering()
            ? render::sampler_mag_filter::linear
            : render::sampler_mag_filter::nearest;

        try {
            batcher_.batch(
                spr.material(),
                property_cache_
                    .sampler(sprite_texture_sampler_hash, render::sampler_state()
                        .texture(texture)
                        .min_filter(min_filter)
                        .mag_filter(mag_filter))
                    .merge(node_r.properties())
                    .merge(internal_properties_),
                indices, E2D_COUNTOF(indices),
                vertices, E2D_COUNTOF(vertices));
        } catch (...) {
            property_cache_.clear();
            throw;
        }
        property_cache_.clear();
    }

    void drawer::context::flush() {
        batcher_.flush();
    }

    //
    // drawer
    //

    drawer::drawer(engine& e, debug& d, render& r)
    : engine_(e)
    , render_(r)
    , batcher_(d, r) {}
}}
