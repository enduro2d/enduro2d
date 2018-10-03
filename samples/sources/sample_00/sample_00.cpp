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
        "#version 120                        \n"
        "                                    \n"
        "attribute vec3 in_pos;              \n"
        "attribute vec2 in_uv0;              \n"
        "attribute vec2 in_uv1;              \n"
        "attribute vec4 in_color;            \n"
        "                                    \n"
        "varying vec2 uv0;                   \n"
        "varying vec2 uv1;                   \n"
        "varying vec4 color;                 \n"
        "                                    \n"
        "void main(){                        \n"
        "  uv0 = in_uv0;                     \n"
        "  uv1 = in_uv1;                     \n"
        "  color = in_color;                 \n"
        "  gl_Position = vec4(in_pos, 1.0);  \n"
        "}";

    const char* fs_source_cstr =
        "#version 120                        \n"
        "                                    \n"
        "varying vec2 uv0;                   \n"
        "varying vec2 uv1;                   \n"
        "varying vec4 color;                 \n"
        "                                    \n"
        "void main(){                        \n"
        "  gl_FragColor = color;             \n"
        "}";

    u16 indices[] = {
        0, 1, 2, 2, 1, 3};

    const vertex vertices[] = {
        {{-0.5f,  0.5f, 0.0f}, {{0.f, 0.f}, {0.f, 0.f}}, {0xFF, 0x00, 0x00, 0xFF}},
        {{-0.5f, -0.5f, 0.0f}, {{0.f, 0.f}, {0.f, 0.f}}, {0x00, 0xFF, 0x00, 0xFF}},
        {{ 0.5f,  0.5f, 0.0f}, {{0.f, 0.f}, {0.f, 0.f}}, {0x00, 0x00, 0xFF, 0xFF}},
        {{ 0.5f, -0.5f, 0.0f}, {{0.f, 0.f}, {0.f, 0.f}}, {0xFF, 0xFF, 0xFF, 0xFF}}};
}

int e2d_main() {
    modules::initialize<debug>();
    modules::initialize<input>();
    modules::initialize<window>(v2u{640, 480}, "Enduro2D", false);
    modules::initialize<render>(the<debug>(), the<window>());

    the<debug>().register_sink<debug_console_sink>();
    the<window>().register_event_listener<window_input_source>(the<input>());

    const auto ps = the<render>().create_shader(
        make_memory_stream(buffer(vs_source_cstr, std::strlen(vs_source_cstr))),
        make_memory_stream(buffer(fs_source_cstr, std::strlen(fs_source_cstr))));

    const auto ib = the<render>().create_index_buffer(
        indices, E2D_COUNTOF(indices), index_buffer::usage::static_draw);

    const auto vb = the<render>().create_vertex_buffer(
        vertices, E2D_COUNTOF(vertices), vertex_buffer::usage::static_draw);

    const keyboard& k = the<input>().keyboard();
    while ( !the<window>().should_close() && !k.is_key_just_released(keyboard_key::escape) ) {
        the<render>().set_clear_color({1.f, 0.4f, 0.f});
        the<render>().clear(true, true, true);
        the<render>().draw(ps, ib, vb);
        the<window>().swap_buffers(true);
        the<input>().frame_tick();
        window::poll_events();
    }
    return 0;
}
