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

        uniform mat4 u_MVP;

        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
          v_color = a_color;
          v_uv = a_uv;

          gl_Position = vec4(a_position, 1.0) * u_MVP;
        }
    )glsl";

    const char* fs_source_cstr = R"glsl(
        #version 120

        uniform sampler2D u_texture;
        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
          vec2 uv = vec2(v_uv.s, 1.0 - v_uv.t);
          gl_FragColor = v_color * texture2D(u_texture, uv);
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

    array<vertex1,24> generate_cube_vertices(const v3f& size) noexcept {
        f32 x = size.x * 0.5f;
        f32 y = size.y * 0.5f;
        f32 z = size.z * 0.5f;
        return {
            vertex1{{-x, -y, -z}, {0, 1}},
            vertex1{{ x, -y, -z}, {1, 1}},
            vertex1{{ x,  y, -z}, {1, 0}},
            vertex1{{-x,  y, -z}, {0, 0}},

            vertex1{{-x, -y,  z}, {0, 1}},
            vertex1{{ x, -y,  z}, {1, 1}},
            vertex1{{ x, -y, -z}, {1, 0}},
            vertex1{{-x, -y, -z}, {0, 0}},

            vertex1{{ x, -y,  z}, {0, 1}},
            vertex1{{-x, -y,  z}, {1, 1}},
            vertex1{{-x,  y,  z}, {1, 0}},
            vertex1{{ x,  y,  z}, {0, 0}},

            vertex1{{-x,  y, -z}, {0, 1}},
            vertex1{{ x,  y, -z}, {1, 1}},
            vertex1{{ x,  y,  z}, {1, 0}},
            vertex1{{-x,  y,  z}, {0, 0}},

            vertex1{{ x, -y, -z}, {0, 1}},
            vertex1{{ x, -y,  z}, {1, 1}},
            vertex1{{ x,  y,  z}, {1, 0}},
            vertex1{{ x,  y, -z}, {0, 0}},

            vertex1{{-x, -y,  z}, {0, 1}},
            vertex1{{-x, -y, -z}, {1, 1}},
            vertex1{{-x,  y, -z}, {1, 0}},
            vertex1{{-x,  y,  z}, {0, 0}}};
    }

    array<vertex2,24> generate_cube_colors() noexcept {
        return {
            vertex2{color32::red()},
            vertex2{color32::green()},
            vertex2{color32::blue()},
            vertex2{color32::yellow()},

            vertex2{color32::red()},
            vertex2{color32::green()},
            vertex2{color32::blue()},
            vertex2{color32::yellow()},

            vertex2{color32::red()},
            vertex2{color32::green()},
            vertex2{color32::blue()},
            vertex2{color32::yellow()},

            vertex2{color32::red()},
            vertex2{color32::green()},
            vertex2{color32::blue()},
            vertex2{color32::yellow()},

            vertex2{color32::red()},
            vertex2{color32::green()},
            vertex2{color32::blue()},
            vertex2{color32::yellow()},

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

    auto texture = the<render>().create_texture(
        the<vfs>().open(url("ships://ship (3).png")));

    const auto shader = the<render>().create_shader(
        vs_source_cstr, fs_source_cstr);

    const auto indices = generate_cube_indices();
    const auto index_buffer = the<render>().create_index_buffer(
        buffer(indices.data(), indices.size() * sizeof(indices[0])),
        index_declaration::index_type::unsigned_byte,
        index_buffer::usage::static_draw);

    const auto vertices1 = generate_cube_vertices(make_vec3(1.f));
    const auto vertex_buffer1 = the<render>().create_vertex_buffer(
        buffer(vertices1.data(), vertices1.size() * sizeof(vertices1[0])),
        vertex1::decl(),
        vertex_buffer::usage::static_draw);

    const auto vertices2 = generate_cube_colors();
    const auto vertex_buffer2 = the<render>().create_vertex_buffer(
        buffer(vertices2.data(), vertices2.size() * sizeof(vertices2[0])),
        vertex2::decl(),
        vertex_buffer::usage::static_draw);

    if ( !texture || !shader || !index_buffer || !vertex_buffer1 || !vertex_buffer2 ) {
        return 1;
    }

    auto material = render::material()
        .add_pass(render::pass_state()
            .states(render::state_block()
                .capabilities(render::capabilities_state()
                    .culling(true)
                    .depth_test(true))
                .culling(render::culling_state()
                    .mode(render::culling_mode::ccw)
                    .face(render::culling_face::back)))
            .shader(shader)
            .properties(render::property_block()
                .sampler("u_texture", render::sampler_state()
                    .texture(texture)
                    .min_filter(render::sampler_min_filter::linear)
                    .mag_filter(render::sampler_mag_filter::linear))));

    auto geometry = render::geometry()
        .indices(index_buffer)
        .add_vertices(vertex_buffer1)
        .add_vertices(vertex_buffer2);

    const auto begin_game_time = time::now_ms();
    const auto framebuffer_size = the<window>().real_size().cast_to<f32>();
    const auto projection = math::make_perspective_lh_matrix4(
        make_deg(45.f),
        framebuffer_size.x / framebuffer_size.y,
        0.1f,
        100.f);

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        const auto game_time = (time::now_ms() - begin_game_time).cast_to<f32>().value;

        const auto MVP =
            math::make_rotation_matrix4(make_rad(game_time) * 0.001f, 1.f, 0.f, 0.f) *
            math::make_rotation_matrix4(make_rad(game_time) * 0.001f, 0.f, 1.f, 0.f) *
            math::make_rotation_matrix4(make_rad(game_time) * 0.001f, 0.f, 0.f, 1.f) *
            math::make_translation_matrix4(0.f, 0.f, 0.f) *
            math::make_loot_at_lh_matrix4({0.f,0.f,-3.f}, v3f::zero(), v3f::unit_y()) *
            projection;

        material.properties()
            .property("u_time", game_time)
            .property("u_MVP", MVP);

        the<render>().execute(render::command_block<64>()
            .add_command(render::clear_command()
                .color_value({1.f, 0.4f, 0.f, 1.f}))
            .add_command(render::draw_command(material, geometry)));

        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }

    return 0;
}
