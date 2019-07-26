/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "render_system_drawer.hpp"

#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>
#include <enduro2d/high/components/spine_renderer.hpp>
#include <enduro2d/high/components/spine_player.hpp>

#include <spine/AnimationState.h>
#include <spine/Skeleton.h>
#include <spine/VertexEffect.h>
#include <spine/SkeletonClipping.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>

namespace
{
    using namespace e2d;

    const str_hash matrix_v_property_hash = "u_matrix_v";
    const str_hash matrix_p_property_hash = "u_matrix_p";
    const str_hash matrix_vp_property_hash = "u_matrix_vp";
    const str_hash matrix_mvp_property_hash = "u_matrix_mvp";
    const str_hash game_time_property_hash = "u_game_time";
    const str_hash sprite_texture_sampler_hash = "u_texture";
}

namespace e2d::render_system_impl
{
    //
    // drawer::context
    //

    drawer::context::context(
        const camera& cam,
        const const_node_iptr& cam_n,
        engine& engine,
        render& render,
        batcher_type& batcher,
        std::vector<batcher_type::vertex_type>& spine_vertices)
    : render_(render)
    , batcher_(batcher)
    , spine_vertices_(spine_vertices)
    {
        const m4f& cam_w = cam_n
            ? cam_n->world_matrix()
            : m4f::identity();
        const std::pair<m4f,bool> cam_w_inv = math::inversed(cam_w);

        view_mat_ = cam_w_inv.second
            ? cam_w_inv.first
            : m4f::identity();
        proj_mat_ = cam.projection();

        batcher_.flush()
            .property(matrix_v_property_hash, view_mat_)
            .property(matrix_p_property_hash, proj_mat_)
            .property(matrix_vp_property_hash, view_mat_ * proj_mat_)
            .property(game_time_property_hash, engine.time());

        render.execute(render::command_block<3>()
            .add_command(render::target_command(cam.target()))
            .add_command(render::viewport_command(cam.viewport()))
            .add_command(render::clear_command()
                .color_value(cam.background())));
    }

    drawer::context::~context() noexcept {
        batcher_.clear(true);
    }

    void drawer::context::draw(
        const const_node_iptr& node)
    {
        if ( !node || !node->owner() ) {
            return;
        }

        E2D_ASSERT(node->owner()->entity().valid());
        ecs::const_entity node_e = node->owner()->entity();
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
            const spine_renderer* spine_r = node_e.find_component<spine_renderer>();
            const spine_player* spine_p = node_e.find_component<spine_player>();
            if ( spine_r ) {
                draw(node, *node_r, *spine_r, spine_p);
            }
        }
    }

    void drawer::context::draw(
        const const_node_iptr& node,
        const renderer& node_r,
        const model_renderer& mdl_r)
    {
        if ( !node || !node_r.enabled() ) {
            return;
        }

        if ( !mdl_r.model() || !mdl_r.model()->content().mesh() ) {
            return;
        }

        const model& mdl = mdl_r.model()->content();
        const mesh& msh = mdl.mesh()->content();

        try {
            property_cache_
                .merge(batcher_.flush())
                .property("u_matrix_m", node->world_matrix())
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
        } catch (...) {
            property_cache_.clear();
            throw;
        }
        property_cache_.clear();
    }

    void drawer::context::draw(
        const const_node_iptr& node,
        const renderer& node_r,
        const sprite_renderer& spr_r)
    {
        if ( !node || !node_r.enabled() ) {
            return;
        }

        if ( !spr_r.sprite() || node_r.materials().empty() ) {
            return;
        }

        const sprite& spr = spr_r.sprite()->content();
        const texture_asset::ptr& tex_a = spr.texture();
        const material_asset::ptr& mat_a = node_r.materials().front();

        if ( !tex_a || !tex_a->content() || !mat_a ) {
            return;
        }

        const b2f& tex_r = spr.texrect();
        const v2f& tex_s = tex_a->content()->size().cast_to<f32>();

        const f32 sw = tex_r.size.x;
        const f32 sh = tex_r.size.y;

        const f32 px = tex_r.position.x - spr.pivot().x;
        const f32 py = tex_r.position.y - spr.pivot().y;

        const v4f p1{px + 0.f, py + 0.f, 0.f, 1.f};
        const v4f p2{px + sw,  py + 0.f, 0.f, 1.f};
        const v4f p3{px + sw,  py + sh,  0.f, 1.f};
        const v4f p4{px + 0.f, py + sh,  0.f, 1.f};

        const f32 tx = tex_r.position.x / tex_s.x;
        const f32 ty = tex_r.position.y / tex_s.y;
        const f32 tw = tex_r.size.x / tex_s.x;
        const f32 th = tex_r.size.y / tex_s.y;

        const m4f& sm = node->world_matrix();
        const color32& tc = spr_r.tint();

        const batcher_type::index_type indices[] = {
            0u, 1u, 2u, 2u, 3u, 0u};

        const batcher_type::vertex_type vertices[] = {
            { v3f(p1 * sm), {tx + 0.f, ty + 0.f}, tc },
            { v3f(p2 * sm), {tx + tw,  ty + 0.f}, tc },
            { v3f(p3 * sm), {tx + tw,  ty + th }, tc },
            { v3f(p4 * sm), {tx + 0.f, ty + th }, tc }};

        const render::sampler_min_filter min_filter = spr_r.filtering()
            ? render::sampler_min_filter::linear
            : render::sampler_min_filter::nearest;

        const render::sampler_mag_filter mag_filter = spr_r.filtering()
            ? render::sampler_mag_filter::linear
            : render::sampler_mag_filter::nearest;

        try {
            property_cache_
                .sampler(sprite_texture_sampler_hash, render::sampler_state()
                    .texture(tex_a->content())
                    .min_filter(min_filter)
                    .mag_filter(mag_filter))
                .merge(node_r.properties());

            batcher_.batch(
                mat_a,
                property_cache_,
                indices, std::size(indices),
                vertices, std::size(vertices));
        } catch (...) {
            property_cache_.clear();
            throw;
        }
        property_cache_.clear();
    }
    
    void drawer::context::draw(
        const const_node_iptr& node,
        const renderer& node_r,
        const spine_renderer& spine_r,
        const spine_player* spine_p)
    {
        static_assert(sizeof(batcher_type::vertex_type) % sizeof(float) == 0, "invalid stride");
        constexpr int stride = sizeof(batcher_type::vertex_type) / sizeof(float);

        if ( !node || !node_r.enabled() ) {
            return;
        }

        if ( node_r.materials().empty() ) {
            return;
        }

        spSkeleton* skeleton = spine_r.skeleton().operator->();
        spAnimationState* anim_state = spine_p ? spine_p->animation().operator->() : nullptr;
        spSkeletonClipping* clipper = spine_r.clipper().operator->();
        spVertexEffect* effect = spine_r.effect().operator->();
        const material_asset::ptr& mat_a = node_r.materials().front();
        const m4f& mvp = node->world_matrix() * view_mat_ * proj_mat_;
        float dt = 0.01f;

        if ( !skeleton || !clipper || !mat_a ) {
            return;
        }

        spSkeleton_update(skeleton, dt);
        if ( anim_state ) {
            spAnimationState_update(anim_state, dt);
            spAnimationState_apply(anim_state, skeleton);
        }
        spSkeleton_updateWorldTransform(skeleton);

    	const u16 quad_indices[6] = { 0, 1, 2, 2, 3, 0 };

        if ( skeleton->color.a == 0 ) {
            return;
        }

        if ( effect ) {
            effect->begin(effect, skeleton);
        }
        

        for ( int i = 0; i < skeleton->slotsCount; ++i ) {
            spSlot* slot = skeleton->drawOrder[i];
            spAttachment* attachment = slot->attachment;
            if ( !attachment ) {
                continue;
            }
            if ( slot->color.a == 0 ) {
                spSkeletonClipping_clipEnd(clipper, slot);
                continue;
            }

            size_t vertex_count = 0;
            const float* uvs = nullptr;
            const u16* indices = nullptr;
            int index_count = 0;
            const spColor* attachment_color;
            texture_ptr texture;

            if ( attachment->type == SP_ATTACHMENT_REGION ) {
                spRegionAttachment* region = reinterpret_cast<spRegionAttachment*>(attachment);
                attachment_color = &region->color;

                if ( attachment_color->a == 0 ) {
                    spSkeletonClipping_clipEnd(clipper, slot);
                    continue;
                }
                spRegionAttachment_computeWorldVertices(region, slot->bone, &spine_vertices_.data()->v.x, 0, stride);
                vertex_count = 4;
                uvs = region->uvs;
                indices = quad_indices;
                index_count = 6;
                if ( texture_asset* asset = static_cast<texture_asset*>(static_cast<spAtlasRegion*>(region->rendererObject)->page->rendererObject) ) {
                    texture = asset->content();
                }
            } else if ( attachment->type == SP_ATTACHMENT_MESH ) {
                spMeshAttachment* mesh = reinterpret_cast<spMeshAttachment*>(attachment);
                attachment_color = &mesh->color;

                if ( attachment_color->a == 0 ) {
                    spSkeletonClipping_clipEnd(clipper, slot);
                    continue;
                }
                vertex_count = mesh->super.worldVerticesLength >> 1;
                if ( vertex_count > spine_vertices_.size() ) {
                    spine_vertices_.resize(vertex_count);
                }
                spVertexAttachment_computeWorldVertices(&mesh->super, slot, 0, mesh->super.worldVerticesLength, &spine_vertices_.data()->v.x, 0, stride);
                uvs = mesh->uvs;
                indices = mesh->triangles;
                index_count = mesh->trianglesCount;
                if ( texture_asset* asset = static_cast<texture_asset*>(static_cast<spAtlasRegion*>(mesh->rendererObject)->page->rendererObject) ) {
                    texture = asset->content();
                }
            } else if ( attachment->type == SP_ATTACHMENT_CLIPPING ) {
                spClippingAttachment* clip = reinterpret_cast<spClippingAttachment*>(attachment);
                spSkeletonClipping_clipStart(clipper, slot, clip);
                E2D_ASSERT(false);
                continue;
            } else {
                continue;
            }

            const color32 vert_color(
                color(skeleton->color.r, skeleton->color.g, skeleton->color.b, skeleton->color.a) *
                color(slot->color.r, slot->color.g, slot->color.b, slot->color.a) *
                color(attachment_color->r, attachment_color->g, attachment_color->b, attachment_color->a));

            switch ( slot->data->blendMode ) {
                case SP_BLEND_MODE_NORMAL :
                    break;
                case SP_BLEND_MODE_ADDITIVE :
                    break;
                case SP_BLEND_MODE_MULTIPLY :
                    break;
                case SP_BLEND_MODE_SCREEN :
                    break;
                default :
                    break;
            }

            /*const float* vertices = spine_vertices_.data();
            if ( spSkeletonClipping_isClipping(clipper) ) {
                spSkeletonClipping_clipTriangles(clipper, vertices, vertex_count, indices, index_count, uvs, 2);
                vertices = clipper->clippedVertices->items;
                vertex_count = clipper->clippedVertices->size >> 1;
                uvs = clipper->clippedUVs->items;
                indices = clipper->clippedTriangles->items;
                index_count = clipper->clippedTriangles->size;
            }*/

            if ( effect ) {
                E2D_ASSERT(false);
            } else {
                for ( int i = 0; i < index_count; ++i ) {
                    int index = indices[i] << 1;
                    auto& vert = spine_vertices_[indices[i]];
                    vert.v.z = 0.0f;
                    vert.t.x = uvs[index];
                    vert.t.y = uvs[index+1];
                    vert.c = vert_color;
                }
            }
            
            try {
                property_cache_
                    .sampler(sprite_texture_sampler_hash, render::sampler_state()
                        .texture(texture)
                        .min_filter(render::sampler_min_filter::linear)
                        .mag_filter(render::sampler_mag_filter::linear))
                    .property(matrix_mvp_property_hash, mvp)
                    .merge(node_r.properties());

                batcher_.batch(
                    mat_a,
                    property_cache_,
                    indices, index_count,
                    spine_vertices_.data(), vertex_count);
            } catch (...) {
                property_cache_.clear();
                throw;
            }

            spSkeletonClipping_clipEnd(clipper, slot);
        }

        spSkeletonClipping_clipEnd2(clipper);

        if ( effect ) {
            effect->end(effect);
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
    , batcher_(d, r) {
        spine_vertices_.resize(256);
    }
}
