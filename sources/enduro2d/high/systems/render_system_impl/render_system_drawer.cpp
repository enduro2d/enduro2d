/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "render_system_drawer.hpp"

#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

#include <spine/spine.h>

namespace
{
    using namespace e2d;

    const str_hash screen_s_property_hash = "u_screen_s";

    const str_hash matrix_m_property_hash = "u_matrix_m";
    const str_hash matrix_v_property_hash = "u_matrix_v";
    const str_hash matrix_p_property_hash = "u_matrix_p";
    const str_hash matrix_vp_property_hash = "u_matrix_vp";

    const str_hash time_property_hash = "u_time";
    const str_hash texture_sampler_hash = "u_texture";

    const str_hash normal_material_hash = "normal";
    const str_hash additive_material_hash = "additive";
    const str_hash multiply_material_hash = "multiply";
    const str_hash screen_material_hash = "screen";
}

namespace e2d::render_system_impl
{
    //
    // drawer::context
    //

    drawer::context::context(
        const camera& cam,
        engine& engine,
        render& render,
        window& window,
        batcher_type& batcher)
    : render_(render)
    , batcher_(batcher)
    {
        const m4f& m_v = cam.view();
        const m4f& m_p = cam.projection();

        batcher_.flush()
            .property(screen_s_property_hash, cam.target()
                ? cam.target()->size().cast_to<f32>()
                : window.framebuffer_size().cast_to<f32>())
            .property(matrix_v_property_hash, m_v)
            .property(matrix_p_property_hash, m_p)
            .property(matrix_vp_property_hash, m_v * m_p)
            .property(time_property_hash, engine.time());

        const v2u target_size = cam.target()
            ? cam.target()->size()
            : window.framebuffer_size();

        const b2f target_viewport = make_rect(
            cam.viewport().position * target_size.cast_to<f32>(),
            cam.viewport().size * target_size.cast_to<f32>());

        render.execute(render::command_block<3>()
            .add_command(render::target_command(
                cam.target()))
            .add_command(render::viewport_command(
                target_viewport.cast_to<i32>(),
                target_viewport.cast_to<i32>()))
            .add_command(render::clear_command()
                .color_value(cam.background())));
    }

    drawer::context::~context() noexcept {
        batcher_.clear(true);
    }

    void drawer::context::draw(const const_node_iptr& node) {
        if ( !node || !node->owner() ) {
            return;
        }

        const gobject& owner = node->owner();
        gcomponent<renderer> node_r{owner};

        if ( !node_r || owner.component<disabled<renderer>>() ) {
            return;
        }

        const m4f& model_m =
            math::make_trs_matrix4(node_r->transform()) *
            node->world_matrix();

        if ( auto mdl_r = gcomponent<model_renderer>{owner} ) {
            draw(model_m, *node_r, *mdl_r);
        }

        if ( auto spn_p = gcomponent<spine_player>{owner} ) {
            draw(model_m, *node_r, *spn_p);
        }

        if ( auto spr_r = gcomponent<sprite_renderer>{owner} ) {
            draw(model_m, *node_r, *spr_r);
        }
    }

    void drawer::context::flush() {
        batcher_.flush();
    }

    void drawer::context::draw(
        const m4f& model_m,
        const renderer& node_r,
        const model_renderer& mdl_r)
    {
        if ( !mdl_r.model() || !mdl_r.model()->content().mesh() ) {
            return;
        }

        const model& mdl = mdl_r.model()->content();
        const mesh& msh = mdl.mesh()->content();

        DEFER([this](){
            property_cache_.clear();
        });

        property_cache_
            .merge(batcher_.flush())
            .property(matrix_m_property_hash, model_m)
            .merge(node_r.properties());

        const std::size_t submesh_count = math::min(
            msh.indices_submesh_count(),
            node_r.materials().size());

        for ( std::size_t i = 0, first_index = 0; i < submesh_count; ++i ) {
            const std::size_t index_count = msh.indices(i).size();
            const material_asset::ptr& mat = node_r.materials()[i];
            if ( mat ) {
                render_.execute(render::draw_command(
                    mat->content(),
                    mdl.geometry(),
                    property_cache_
                ).index_range(first_index, index_count));
            }
            first_index += index_count;
        }
    }

    void drawer::context::draw(
        const m4f& model_m,
        const renderer& node_r,
        const spine_player& spine_r)
    {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local std::vector<float> temp_vertices(1000u, 0.f);
        static thread_local std::vector<batcher_type::vertex_type> batch_vertices(1000u);

        spSkeleton* skeleton = spine_r.skeleton().get();
        spSkeletonClipping* clipper = spine_r.clipper().get();

        if ( !skeleton || !clipper ) {
            return;
        }

        if ( math::is_near_zero(skeleton->color.a) ) {
            return;
        }

        material_asset::ptr normal_mat_a;
        material_asset::ptr additive_mat_a;
        material_asset::ptr multiply_mat_a;
        material_asset::ptr screen_mat_a;

        unsigned short quad_indices[6] = { 0, 1, 2, 2, 3, 0 };

        DEFER([this, clipper](){
            property_cache_.clear();
            spSkeletonClipping_clipEnd2(clipper);
        });

        for ( int i = 0; i < skeleton->slotsCount; ++i ) {
            spSlot* slot = skeleton->drawOrder[i];

            auto slot_clipping_defer = defer_hpp::make_defer([clipper, slot](){
                spSkeletonClipping_clipEnd(clipper, slot);
            });

            spAttachment* attachment = slot->attachment;
            if ( !attachment || math::is_near_zero(slot->color.a) ) {
                continue;
            }

            float* uvs = nullptr;
            unsigned short* indices = nullptr;
            int index_count = 0;
            int vertex_count = 0;
            const spAtlasPage* atlas_page = nullptr;
            const spColor* attachment_color = nullptr;

            if ( attachment->type == SP_ATTACHMENT_REGION ) {
                spRegionAttachment* region = reinterpret_cast<spRegionAttachment*>(attachment);

                attachment_color = &region->color;
                if ( math::is_near_zero(attachment_color->a) ) {
                    continue;
                }

                {
                    vertex_count = 8;
                    if ( temp_vertices.size() < math::numeric_cast<std::size_t>(vertex_count) ) {
                        temp_vertices.resize(math::max(
                            temp_vertices.size() * 2u,
                            math::numeric_cast<std::size_t>(vertex_count)));
                    }
                }

                spRegionAttachment_computeWorldVertices(
                    region,
                    slot->bone,
                    temp_vertices.data(),
                    0, 2);

                uvs = region->uvs;
                indices = quad_indices;
                index_count = 6;
                atlas_page = static_cast<spAtlasRegion*>(region->rendererObject)->page;
            } else if ( attachment->type == SP_ATTACHMENT_MESH ) {
                spMeshAttachment* mesh = reinterpret_cast<spMeshAttachment*>(attachment);

                attachment_color = &mesh->color;
                if ( math::is_near_zero(attachment_color->a) ) {
                    continue;
                }

                {
                    vertex_count = mesh->super.worldVerticesLength;
                    if ( temp_vertices.size() < math::numeric_cast<std::size_t>(vertex_count) ) {
                        temp_vertices.resize(math::max(
                            temp_vertices.size() * 2u,
                            math::numeric_cast<std::size_t>(vertex_count)));
                    }
                }

                spVertexAttachment_computeWorldVertices(
                    &mesh->super,
                    slot,
                    0,
                    mesh->super.worldVerticesLength,
                    temp_vertices.data(),
                    0, 2);

                uvs = mesh->uvs;
                indices = mesh->triangles;
                index_count = mesh->trianglesCount;
                atlas_page = static_cast<spAtlasRegion*>(mesh->rendererObject)->page;
            } else if ( attachment->type == SP_ATTACHMENT_CLIPPING ) {
                spClippingAttachment* clip = reinterpret_cast<spClippingAttachment*>(attachment);
                spSkeletonClipping_clipStart(clipper, slot, clip);
                slot_clipping_defer.dismiss();
                continue;
            } else {
                slot_clipping_defer.dismiss();
                continue;
            }

            color32 vert_color = color32(
                color(skeleton->color.r, skeleton->color.g, skeleton->color.b, skeleton->color.a) *
                color(slot->color.r, slot->color.g, slot->color.b, slot->color.a) *
                color(attachment_color->r, attachment_color->g, attachment_color->b, attachment_color->a));

            texture_ptr tex_p;
            const texture_asset* texture_asset_ptr = atlas_page
                ? static_cast<const texture_asset*>(atlas_page->rendererObject)
                : nullptr;
            if ( texture_asset_ptr ) {
                tex_p = texture_asset_ptr->content();
            }

            render::sampler_min_filter tex_min_f = render::sampler_min_filter::linear;
            if ( atlas_page ) {
                switch ( atlas_page->minFilter ) {
                case SP_ATLAS_NEAREST:
                case SP_ATLAS_MIPMAP_NEAREST_LINEAR:
                case SP_ATLAS_MIPMAP_NEAREST_NEAREST:
                    tex_min_f = render::sampler_min_filter::nearest;
                    break;
                default:
                    tex_min_f = render::sampler_min_filter::linear;
                    break;
                }
            }

            render::sampler_mag_filter tex_mag_f = render::sampler_mag_filter::linear;
            if ( atlas_page ) {
                switch ( atlas_page->magFilter ) {
                case SP_ATLAS_NEAREST:
                case SP_ATLAS_MIPMAP_NEAREST_LINEAR:
                case SP_ATLAS_MIPMAP_NEAREST_NEAREST:
                    tex_mag_f = render::sampler_mag_filter::nearest;
                    break;
                default:
                    tex_mag_f = render::sampler_mag_filter::linear;
                    break;
                }
            }

            render::sampler_wrap tex_wrap_s = render::sampler_wrap::repeat;
            if ( atlas_page ) {
                switch ( atlas_page->uWrap ) {
                case SP_ATLAS_MIRROREDREPEAT:
                    tex_wrap_s = render::sampler_wrap::mirror;
                    break;
                case SP_ATLAS_CLAMPTOEDGE:
                    tex_wrap_s = render::sampler_wrap::clamp;
                    break;
                case SP_ATLAS_REPEAT:
                    tex_wrap_s = render::sampler_wrap::repeat;
                    break;
                default:
                    E2D_ASSERT_MSG(false, "unexpected wrap mode for slot");
                    break;
                }
            }

            render::sampler_wrap tex_wrap_t = render::sampler_wrap::repeat;
            if ( atlas_page ) {
                switch ( atlas_page->vWrap ) {
                case SP_ATLAS_MIRROREDREPEAT:
                    tex_wrap_t = render::sampler_wrap::mirror;
                    break;
                case SP_ATLAS_CLAMPTOEDGE:
                    tex_wrap_t = render::sampler_wrap::clamp;
                    break;
                case SP_ATLAS_REPEAT:
                    tex_wrap_t = render::sampler_wrap::repeat;
                    break;
                default:
                    E2D_ASSERT_MSG(false, "unexpected wrap mode for slot");
                    break;
                }
            }

            material_asset::ptr mat_a;
            switch ( slot->data->blendMode ) {
                case SP_BLEND_MODE_NORMAL:
                    mat_a = normal_mat_a
                        ? normal_mat_a
                        : (normal_mat_a = spine_r.find_material(normal_material_hash));
                    break;
                case SP_BLEND_MODE_ADDITIVE:
                    mat_a = additive_mat_a
                        ? additive_mat_a
                        : (additive_mat_a = spine_r.find_material(additive_material_hash));
                    break;
                case SP_BLEND_MODE_MULTIPLY:
                    mat_a = multiply_mat_a
                        ? multiply_mat_a
                        : (multiply_mat_a = spine_r.find_material(multiply_material_hash));
                    break;
                case SP_BLEND_MODE_SCREEN:
                    mat_a = screen_mat_a
                        ? screen_mat_a
                        : (screen_mat_a = spine_r.find_material(screen_material_hash));
                    break;
                default:
                    E2D_ASSERT_MSG(false, "unexpected blend mode for slot");
                    break;
            }

            if ( math::is_near_zero(vert_color.a) || !tex_p || !mat_a ) {
                continue;
            }

            const float* vertices = temp_vertices.data();
            if ( spSkeletonClipping_isClipping(clipper) ) {
                spSkeletonClipping_clipTriangles(
                    clipper,
                    temp_vertices.data(), vertex_count,
                    indices, index_count,
                    uvs,
                    2);
                vertices = clipper->clippedVertices->items;
                vertex_count = clipper->clippedVertices->size;
                uvs = clipper->clippedUVs->items;
                indices = clipper->clippedTriangles->items;
                index_count = clipper->clippedTriangles->size;
            }

            {
                const std::size_t batch_vertex_count = vertex_count >> 1;

                if ( batch_vertices.size() < batch_vertex_count ) {
                    batch_vertices.resize(math::max(batch_vertices.size() * 2u, batch_vertex_count));
                }

                for ( std::size_t j = 0; j < batch_vertex_count; ++j ) {
                    batcher_type::vertex_type& vert = batch_vertices[j];
                    vert.v = v3f(v4f(vertices[j * 2], vertices[j * 2 + 1], 0.f, 1.f) * model_m);
                    vert.t = v2f(uvs[j * 2], uvs[j * 2 + 1]);
                    vert.c = vert_color;
                }

                property_cache_
                    .sampler(texture_sampler_hash, render::sampler_state()
                        .texture(tex_p)
                        .filter(tex_min_f, tex_mag_f)
                        .wrap(tex_wrap_s, tex_wrap_t))
                    .merge(node_r.properties());

                batcher_.batch(
                    mat_a,
                    property_cache_,
                    indices, math::numeric_cast<std::size_t>(index_count),
                    batch_vertices.data(), batch_vertex_count);
            }
        }
    }

    void drawer::context::draw(
        const m4f& model_m,
        const renderer& node_r,
        const sprite_renderer& spr_r)
    {
        if ( !spr_r.sprite() ) {
            return;
        }

        const sprite& spr = spr_r.sprite()->content();
        const texture_ptr& tex_p = spr.texture()
            ? spr.texture()->content()
            : texture_ptr();

        if ( !tex_p || math::is_near_zero(spr_r.tint().a) ) {
            return;
        }

        const render::sampler_min_filter tex_min_f = spr_r.filtering()
            ? render::sampler_min_filter::linear
            : render::sampler_min_filter::nearest;

        const render::sampler_mag_filter tex_mag_f = spr_r.filtering()
            ? render::sampler_mag_filter::linear
            : render::sampler_mag_filter::nearest;

        material_asset::ptr mat_a;
        switch ( spr_r.blending() ) {
        case sprite_renderer::blendings::normal:
            mat_a = spr_r.find_material(normal_material_hash);
            break;
        case sprite_renderer::blendings::additive:
            mat_a = spr_r.find_material(additive_material_hash);
            break;
        case sprite_renderer::blendings::multiply:
            mat_a = spr_r.find_material(multiply_material_hash);
            break;
        case sprite_renderer::blendings::screen:
            mat_a = spr_r.find_material(screen_material_hash);
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected sprite blending");
            break;
        }

        if ( !mat_a ) {
            return;
        }

        DEFER([this](){
            property_cache_.clear();
        });

        property_cache_
            .sampler(texture_sampler_hash, render::sampler_state()
                .texture(tex_p)
                .filter(tex_min_f, tex_mag_f))
            .merge(node_r.properties());

        if ( spr_r.mode() == sprite_renderer::modes::simple ) {

            // 2 -------- 3
            // |          |
            // |          |
            // |          |
            // 0 -------- 1

            const v2f& tex_s = tex_p->size().cast_to<f32>();
            const b2f& outer_r = spr.outer_texrect();

            const v2f size = outer_r.size * spr_r.scale();

            const v2f pos_xs = v2f{
                0.f, size.x};

            const v2f pos_ys = v2f{
                0.f, size.y};

            const v2f tex_xs = v2f{
                outer_r.position.x,
                outer_r.position.x + outer_r.size.x} / tex_s.x;

            const v2f tex_ys = v2f{
                outer_r.position.y,
                outer_r.position.y + outer_r.size.y} / tex_s.y;

            const color32& tc = spr_r.tint();

            const batcher_type::index_type indices[] = {
                0, 1, 3, 3, 2, 0,
            };

            const batcher_type::vertex_type vertices[] = {
                { v3f{v4f{pos_xs[0], pos_ys[0], 0.f, 1.f} * model_m}, v2f{tex_xs[0], tex_ys[0]}, tc },
                { v3f{v4f{pos_xs[1], pos_ys[0], 0.f, 1.f} * model_m}, v2f{tex_xs[1], tex_ys[0]}, tc },
                { v3f{v4f{pos_xs[0], pos_ys[1], 0.f, 1.f} * model_m}, v2f{tex_xs[0], tex_ys[1]}, tc },
                { v3f{v4f{pos_xs[1], pos_ys[1], 0.f, 1.f} * model_m}, v2f{tex_xs[1], tex_ys[1]}, tc },
            };

            batcher_.batch(
                mat_a,
                property_cache_,
                indices, std::size(indices),
                vertices, std::size(vertices));
        } else if ( spr_r.mode() == sprite_renderer::modes::sliced ) {

            // 12 13 ********* 14 15
            // _8 _9 --------- 10 11
            //  |  *           *  |
            //  |  *           *  |
            //  |  *           *  |
            // _4 _5 ********* _6 _7
            // _0 _1 --------- _2 _3

            const v2f& tex_s = tex_p->size().cast_to<f32>();
            const b2f& inner_r = spr.inner_texrect();
            const b2f& outer_r = spr.outer_texrect();

            const f32 left = inner_r.position.x - outer_r.position.x;
            const f32 right = (outer_r.size.x - inner_r.size.x) - left;
            const f32 bottom = inner_r.position.y - outer_r.position.y;
            const f32 top = (outer_r.size.y - inner_r.size.y) - bottom;

            const v2f size = outer_r.size * spr_r.scale();

            const f32 sides_width = left + right;
            const f32 sides_height = bottom + top;

            const f32 max_left = sides_width > 0.f ? size.x * (left / sides_width) : 0.f;
            const f32 max_right = sides_width > 0.f ? size.x * (right / sides_width) : 0.f;
            const f32 max_bottom = sides_height > 0.f ? size.y * (bottom / sides_height) : 0.f;
            const f32 max_top = sides_height > 0.f ? size.y * (top / sides_height) : 0.f;

            const f32 adj_left = math::min(left, max_left);
            const f32 adj_right = math::min(right, max_right);
            const f32 adj_bottom = math::min(bottom, max_bottom);
            const f32 adj_top = math::min(top, max_top);

            const v4f pos_xs = v4f{
                0.f,
                adj_left,
                size.x - adj_right,
                size.x};

            const v4f pos_ys = v4f{
                0.f,
                adj_bottom,
                size.y - adj_top,
                size.y};

            const v4f tex_xs = v4f{
                outer_r.position.x,
                outer_r.position.x + adj_left,
                outer_r.position.x + outer_r.size.x - adj_right,
                outer_r.position.x + outer_r.size.x} / tex_s.x;

            const v4f tex_ys = v4f{
                outer_r.position.y,
                outer_r.position.y + adj_bottom,
                outer_r.position.y + outer_r.size.y - adj_top,
                outer_r.position.y + outer_r.size.y} / tex_s.y;

            const color32& tc = spr_r.tint();

            const batcher_type::index_type indices[] = {
                0, 1, 5, 5, 4, 0,
                1, 2, 6, 6, 5, 1,
                2, 3, 7, 7, 6, 2,

                4, 5, 9, 9, 8, 4,
                5, 6, 10, 10, 9, 5,
                6, 7, 11, 11, 10, 6,

                8, 9, 13, 13, 12, 8,
                9, 10, 14, 14, 13, 9,
                10, 11, 15, 15, 14, 10,
            };

            const batcher_type::vertex_type vertices[] = {
                { v3f{v4f{pos_xs[0], pos_ys[0], 0.f, 1.f} * model_m}, v2f{tex_xs[0], tex_ys[0]}, tc },
                { v3f{v4f{pos_xs[1], pos_ys[0], 0.f, 1.f} * model_m}, v2f{tex_xs[1], tex_ys[0]}, tc },
                { v3f{v4f{pos_xs[2], pos_ys[0], 0.f, 1.f} * model_m}, v2f{tex_xs[2], tex_ys[0]}, tc },
                { v3f{v4f{pos_xs[3], pos_ys[0], 0.f, 1.f} * model_m}, v2f{tex_xs[3], tex_ys[0]}, tc },

                { v3f{v4f{pos_xs[0], pos_ys[1], 0.f, 1.f} * model_m}, v2f{tex_xs[0], tex_ys[1]}, tc },
                { v3f{v4f{pos_xs[1], pos_ys[1], 0.f, 1.f} * model_m}, v2f{tex_xs[1], tex_ys[1]}, tc },
                { v3f{v4f{pos_xs[2], pos_ys[1], 0.f, 1.f} * model_m}, v2f{tex_xs[2], tex_ys[1]}, tc },
                { v3f{v4f{pos_xs[3], pos_ys[1], 0.f, 1.f} * model_m}, v2f{tex_xs[3], tex_ys[1]}, tc },

                { v3f{v4f{pos_xs[0], pos_ys[2], 0.f, 1.f} * model_m}, v2f{tex_xs[0], tex_ys[2]}, tc },
                { v3f{v4f{pos_xs[1], pos_ys[2], 0.f, 1.f} * model_m}, v2f{tex_xs[1], tex_ys[2]}, tc },
                { v3f{v4f{pos_xs[2], pos_ys[2], 0.f, 1.f} * model_m}, v2f{tex_xs[2], tex_ys[2]}, tc },
                { v3f{v4f{pos_xs[3], pos_ys[2], 0.f, 1.f} * model_m}, v2f{tex_xs[3], tex_ys[2]}, tc },

                { v3f{v4f{pos_xs[0], pos_ys[3], 0.f, 1.f} * model_m}, v2f{tex_xs[0], tex_ys[3]}, tc },
                { v3f{v4f{pos_xs[1], pos_ys[3], 0.f, 1.f} * model_m}, v2f{tex_xs[1], tex_ys[3]}, tc },
                { v3f{v4f{pos_xs[2], pos_ys[3], 0.f, 1.f} * model_m}, v2f{tex_xs[2], tex_ys[3]}, tc },
                { v3f{v4f{pos_xs[3], pos_ys[3], 0.f, 1.f} * model_m}, v2f{tex_xs[3], tex_ys[3]}, tc },
            };

            batcher_.batch(
                mat_a,
                property_cache_,
                indices, std::size(indices),
                vertices, std::size(vertices));
        } else {
            E2D_ASSERT_MSG(false, "unexpected sprite mode");
        }
    }

    //
    // drawer
    //

    drawer::drawer(engine& e, debug& d, render& r, window& w)
    : engine_(e)
    , render_(r)
    , window_(w)
    , batcher_(d, r) {}
}
