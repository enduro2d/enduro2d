/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    const char* vs_source_cstr =
        "#version 120                                       \n"
        "                                                   \n"
        "attribute vec3 a_position;                         \n"
        "attribute vec4 a_color;                            \n"
        "                                                   \n"
        "uniform float u_time = 0.0;                        \n"
        "                                                   \n"
        "varying vec4 color;                                \n"
        "                                                   \n"
        "void main(){                                       \n"
        "  color = a_color;                                 \n"
        "                                                   \n"
        "  float s = 0.7 + 0.3 * (cos(u_time * 0.003) + 1); \n"
        "  gl_Position = vec4(a_position * s, 1.0);         \n"
        "}";

    const char* fs_source_cstr =
        "#version 120            \n"
        "                        \n"
        "varying vec4 color;     \n"
        "                        \n"
        "void main(){            \n"
        "  gl_FragColor = color; \n"
        "}";

    struct vertex {
        v3f position;
        color32 color;
    };

    u8 indices[] = {
        0, 1, 2, 2, 1, 3};

    const vertex vertices[] = {
        {{-0.5f,  0.5f, 0.0f}, color32::red()},
        {{-0.5f, -0.5f, 0.0f}, color32::green()},
        {{ 0.5f,  0.5f, 0.0f}, color32::blue()},
        {{ 0.5f, -0.5f, 0.0f}, color32::white()}};
}

int e2d_main() {
    modules::initialize<debug>()
        .register_sink<debug_console_sink>();
    modules::initialize<input>();
    modules::initialize<window>(v2u{640, 480}, "Enduro2D", false)
        .register_event_listener<window_input_source>(the<input>());
    modules::initialize<render>(the<debug>(), the<window>());

    auto index_decl = index_declaration(
        index_declaration::index_type::unsigned_byte);

    auto vertex_decl = vertex_declaration()
        .add_attribute<v3f>("a_position")
        .add_attribute<color32>("a_color").normalized();

    const auto ps = the<render>().create_shader(
        make_memory_stream(buffer(vs_source_cstr, std::strlen(vs_source_cstr))),
        make_memory_stream(buffer(fs_source_cstr, std::strlen(fs_source_cstr))),
        vertex_decl);

    const auto ib = the<render>().create_index_buffer(
        buffer(indices, E2D_COUNTOF(indices) * sizeof(indices[0])),
        index_decl,
        index_buffer::usage::static_draw);

    const auto vb = the<render>().create_vertex_buffer(
        buffer(vertices, E2D_COUNTOF(vertices) * sizeof(vertices[0])),
        vertex_decl,
        vertex_buffer::usage::static_draw);

    const auto begin_time = time::now_ms();

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        {
            f32 t = (time::now_ms() - begin_time).cast_to<f32>().value;
            ps->set_uniform("u_time", t);
        }
        {
            the<render>()
                .clear_depth_buffer(1.f)
                .clear_stencil_buffer(0)
                .clear_color_buffer({1.f, 0.4f, 0.f, 1.f})
                .draw(render::topology::triangles, ps, ib, vb);
        }
        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }
    return 0;
}
