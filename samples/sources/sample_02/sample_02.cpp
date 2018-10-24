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

        uniform mat4 u_MVP;

        varying vec2 v_uv;

        void main(){
          v_uv = a_uv;

          gl_Position = vec4(a_position, 1.0) * u_MVP;
        }
    )glsl";

    const char* fs_source_cstr = R"glsl(
        #version 120

        uniform sampler2D u_texture;
        varying vec2 v_uv;

        void main(){
          vec2 uv = vec2(v_uv.s, 1.0 - v_uv.t);
          gl_FragColor = texture2D(u_texture, uv);
        }
    )glsl";

    struct vertex {
        v3f position;
        v2hu uv;
        static vertex_declaration decl() noexcept {
            return vertex_declaration()
                .add_attribute<v3f>("a_position")
                .add_attribute<v2hu>("a_uv");
        }
    };

    array<u8,36> generate_cube_indices() noexcept {
        return {
            0,  1,  2,
            2,  3,  0,

            4,  5,  6,
            6,  7,  4,

            8,  9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20};
    }

    array<vertex,24> generate_cube_vertices(const v3f& size) noexcept {
        f32 x = size.x * 0.5f;
        f32 y = size.y * 0.5f;
        f32 z = size.z * 0.5f;
        return {
            vertex{{-x, -y, -z}, {0, 1}},
            vertex{{ x, -y, -z}, {1, 1}},
            vertex{{ x,  y, -z}, {1, 0}},
            vertex{{-x,  y, -z}, {0, 0}},

            vertex{{-x, -y,  z}, {0, 1}},
            vertex{{ x, -y,  z}, {1, 1}},
            vertex{{ x, -y, -z}, {1, 0}},
            vertex{{-x, -y, -z}, {0, 0}},

            vertex{{ x, -y,  z}, {0, 1}},
            vertex{{-x, -y,  z}, {1, 1}},
            vertex{{-x,  y,  z}, {1, 0}},
            vertex{{ x,  y,  z}, {0, 0}},

            vertex{{-x,  y, -z}, {0, 1}},
            vertex{{ x,  y, -z}, {1, 1}},
            vertex{{ x,  y,  z}, {1, 0}},
            vertex{{-x,  y,  z}, {0, 0}},

            vertex{{ x, -y, -z}, {0, 1}},
            vertex{{ x, -y,  z}, {1, 1}},
            vertex{{ x,  y,  z}, {1, 0}},
            vertex{{ x,  y, -z}, {0, 0}},

            vertex{{-x, -y,  z}, {0, 1}},
            vertex{{-x, -y, -z}, {1, 1}},
            vertex{{-x,  y, -z}, {1, 0}},
            vertex{{-x,  y,  z}, {0, 0}}};
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

    auto texture = the<render>().create_texture(
        the<vfs>().open(url("ships://ship (3).png")));

    const auto shader = the<render>().create_shader(
        vs_source_cstr, fs_source_cstr);

    const auto indices = generate_cube_indices();
    const auto index_buffer = the<render>().create_index_buffer(
        buffer(indices.data(), indices.size() * sizeof(indices[0])),
        index_declaration::index_type::unsigned_byte,
        index_buffer::usage::static_draw);

    const auto vertices = generate_cube_vertices(make_vec3(1.f));
    const auto vertex_buffer = the<render>().create_vertex_buffer(
        buffer(vertices.data(), vertices.size() * sizeof(vertices[0])),
        vertex::decl(),
        vertex_buffer::usage::static_draw);

    if ( !texture || !shader || !index_buffer || !vertex_buffer ) {
        return 1;
    }

    auto material = render::material()
        .add_pass(render::pass_state()
            .states(render::state_block()
                .capabilities(render::capabilities_state()
                    .blending(true)
                    .culling(true)
                    .depth_test(true))
                .blending(render::blending_state()
                    .src_factor(render::blending_factor::src_alpha)
                    .dst_factor(render::blending_factor::one_minus_src_alpha))
                .culling(render::culling_state()
                    .mode(render::culling_mode::ccw)
                    .face(render::culling_face::back)))
            .shader(shader))
        .properties(render::property_block()
            .sampler("u_texture", render::sampler_state()
                .texture(texture)
                .min_filter(render::sampler_min_filter::linear)
                .mag_filter(render::sampler_mag_filter::linear)));

    auto geometry = render::geometry()
        .indices(index_buffer)
        .add_vertices(vertex_buffer);

    const auto begin_game_time = time::now_ms();
    const auto framebuffer_size = the<window>().real_size().cast_to<f32>();
    const auto projection = math::make_perspective_lh_matrix4(
        make_deg(45.f),
        framebuffer_size.x / framebuffer_size.y,
        0.1f,
        100.f);

    const auto rt = the<render>().create_render_target(
        the<window>().real_size() / 10u,
        pixel_declaration::pixel_type::rgba8,
        pixel_declaration::pixel_type::depth16,
        render_target::external_texture::color_and_depth);

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        const auto game_time = (time::now_ms() - begin_game_time).cast_to<f32>().value;

        const auto MVP =
            math::make_rotation_matrix4(make_rad(game_time) * 0.001f, 1.f, 0.f, 0.f) *
            math::make_rotation_matrix4(make_rad(game_time) * 0.001f, 0.f, 1.f, 0.f) *
            math::make_rotation_matrix4(make_rad(game_time) * 0.001f, 0.f, 0.f, 1.f) *
            math::make_translation_matrix4(0.f, 0.f, 0.f) *
            math::make_loot_at_lh_matrix4({0.f,0.f,-2.f}, v3f::zero(), v3f::unit_y()) *
            projection;

        material.properties()
            .property("u_time", game_time)
            .property("u_MVP", MVP);

        material.properties()
            .sampler("u_texture", render::sampler_state()
                .texture(texture)
                .min_filter(render::sampler_min_filter::linear)
                .mag_filter(render::sampler_mag_filter::linear));

        the<render>()
            .set_render_target(rt)
            .set_viewport(rt->size())
            .clear_depth_buffer(1.f)
            .clear_stencil_buffer(0)
            .clear_color_buffer({0.f, 0.4f, 0.f, 1.f})
            .draw(material, geometry);

        material.properties()
            .sampler("u_texture", render::sampler_state()
                .texture(rt->color())
                .min_filter(render::sampler_min_filter::linear)
                .mag_filter(render::sampler_mag_filter::linear));

        the<render>()
            .set_render_target(nullptr)
            .set_viewport(the<window>().real_size())
            .clear_depth_buffer(1.f)
            .clear_stencil_buffer(0)
            .clear_color_buffer({1.f, 0.4f, 0.f, 1.f})
            .draw(material, geometry);

        std::this_thread::sleep_for(time::to_chrono(make_milliseconds(10)));

        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }

    return 0;
}
