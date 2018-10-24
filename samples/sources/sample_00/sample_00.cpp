/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    const char* vs_source_cstr = R"glsl(
        #version 120

        attribute vec3 a_position;
        attribute vec2 a_uv;
        attribute vec4 a_color;

        uniform float u_time;
        uniform mat4 u_MVP;

        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
          v_color = a_color;
          v_uv = a_uv;

          float s = 0.7 + 0.3 * (cos(u_time * 0.003) + 1);
          gl_Position = vec4(a_position * s, 1.0) * u_MVP;
        }
    )glsl";

    const char* fs_source_cstr = R"glsl(
        #version 120

        uniform float u_time;
        uniform sampler2D u_texture1;
        uniform sampler2D u_texture2;
        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
          vec2 uv = vec2(v_uv.s, 1.0 - v_uv.t);
          if ( u_time > 2000 ) {
            gl_FragColor = v_color * texture2D(u_texture2, uv);
          } else {
            gl_FragColor = v_color * texture2D(u_texture1, uv);
          }
        }
    )glsl";

    struct vertex1 {
        v3f position;
        v2hu uv;
        static vertex_declaration decl() noexcept {
            return vertex_declaration()
                .add_attribute<v3f>("a_position")
                .add_attribute<v2hu>("a_uv");
        }
    };

    struct vertex2 {
        color32 color;
        static vertex_declaration decl() noexcept {
            return vertex_declaration()
                .add_attribute<color32>("a_color").normalized();
        }
    };

    array<u8,6> generate_quad_indices() noexcept {
        return {0, 1, 2, 2, 1, 3};
    }

    array<vertex1,4> generate_quad_vertices(const v2u& size) noexcept {
        f32 hw = size.x * 0.5f;
        f32 hh = size.y * 0.5f;
        return {
            vertex1{{-hw,  hh, 0.f}, {0, 1}},
            vertex1{{-hw, -hh, 0.f}, {0, 0}},
            vertex1{{ hw,  hh, 0.f}, {1, 1}},
            vertex1{{ hw, -hh, 0.f}, {1, 0}}};
    }

    array<vertex2,4> generate_quad_colors() noexcept {
        return {
            vertex2{color32::red()},
            vertex2{color32::green()},
            vertex2{color32::blue()},
            vertex2{color32::yellow()}};
    }
}

int e2d_main() {
    {
        modules::initialize<vfs>()
            .register_scheme<filesystem_file_source>("file");
        modules::initialize<debug>()
            .register_sink<debug_console_sink>();
        modules::initialize<input>();
        modules::initialize<window>(v2u{640, 480}, "Enduro2D", false)
            .register_event_listener<window_input_source>(the<input>());
        modules::initialize<render>(the<debug>(), the<window>());
    }
    {
        str resources;
        filesystem::extract_predef_path(resources, filesystem::predef_path::resources);
        the<vfs>().register_scheme_alias(
            "resources",
            url{"file", resources});
        the<vfs>().register_scheme<archive_file_source>(
            "piratepack",
            the<vfs>().open(url("resources://bin/kenney_piratepack.zip")));
        the<vfs>().register_scheme_alias("ships", url("piratepack://PNG/Retina/Ships"));
    }

    auto texture1 = the<render>().create_texture(
        the<vfs>().open(url("ships://ship (2).png")));
    auto texture2 = the<render>().create_texture(
        the<vfs>().open(url("ships://ship (19).png")));

    const auto shader = the<render>().create_shader(
        vs_source_cstr, fs_source_cstr);

    const auto indices = generate_quad_indices();
    const auto index_buffer = the<render>().create_index_buffer(
        buffer(indices.data(), indices.size() * sizeof(indices[0])),
        index_declaration::index_type::unsigned_byte,
        index_buffer::usage::static_draw);

    const auto vertices1 = generate_quad_vertices(texture1->size());
    const auto vertex_buffer1 = the<render>().create_vertex_buffer(
        buffer(vertices1.data(), vertices1.size() * sizeof(vertices1[0])),
        vertex1::decl(),
        vertex_buffer::usage::static_draw);

    const auto vertices2 = generate_quad_colors();
    const auto vertex_buffer2 = the<render>().create_vertex_buffer(
        buffer(vertices2.data(), vertices2.size() * sizeof(vertices2[0])),
        vertex2::decl(),
        vertex_buffer::usage::static_draw);

    if ( !texture1 || !texture2 || !shader || !index_buffer || !vertex_buffer1 || !vertex_buffer2 ) {
        return 1;
    }

    auto material = render::material()
        .add_pass(render::pass_state()
            .states(render::state_block()
                .capabilities(render::capabilities_state()
                    .blending(true))
                .blending(render::blending_state()
                    .src_factor(render::blending_factor::src_alpha)
                    .dst_factor(render::blending_factor::one_minus_src_alpha)))
            .shader(shader)
            .properties(render::property_block()
                .sampler("u_texture1", render::sampler_state()
                    .texture(texture1)
                    .min_filter(render::sampler_min_filter::linear)
                    .mag_filter(render::sampler_mag_filter::linear))
                .sampler("u_texture2", render::sampler_state()
                    .texture(texture2)
                    .min_filter(render::sampler_min_filter::linear)
                    .mag_filter(render::sampler_mag_filter::linear))));

    auto geometry = render::geometry()
        .indices(index_buffer)
        .add_vertices(vertex_buffer1)
        .add_vertices(vertex_buffer2);

    const auto begin_game_time = time::now_ms();
    const auto framebuffer_size = the<window>().real_size().cast_to<f32>();
    const auto projection = math::make_orthogonal_lh_matrix4(framebuffer_size, 0.f, 1.f);

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        const auto game_time = (time::now_ms() - begin_game_time).cast_to<f32>().value;

        material.properties()
            .property("u_time", game_time)
            .property("u_MVP", projection);

        the<render>()
            .clear_depth_buffer(1.f)
            .clear_stencil_buffer(0)
            .clear_color_buffer({1.f, 0.4f, 0.f, 1.f})
            .draw(material, geometry);

        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }

    return 0;
}
