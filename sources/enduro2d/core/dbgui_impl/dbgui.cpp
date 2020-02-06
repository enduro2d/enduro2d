/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "dbgui.hpp"

namespace
{
    using namespace e2d;

    const char* imgui_vertex_source_cstr = R"glsl(
        attribute vec2 a_position;
        attribute vec2 a_uv;
        attribute vec4 a_color;

        uniform mat4 u_MVP;

        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
            v_color = a_color;
            v_uv = a_uv;
            gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0) * u_MVP;
        }
    )glsl";

    const char* imgui_fragment_source_cstr = R"glsl(
        uniform sampler2D u_texture;
        varying vec4 v_color;
        varying vec2 v_uv;

        void main(){
            float glyph_alpha = v_color.a * texture2D(u_texture, v_uv).a;
            gl_FragColor = vec4(v_color.rgb * glyph_alpha, glyph_alpha);
        }
    )glsl";
}

namespace e2d::imgui
{
    const char* vertex_source_cstr() noexcept {
        return imgui_vertex_source_cstr;
    }

    const char* fragment_source_cstr() noexcept {
        return imgui_fragment_source_cstr;
    }

    window::cursor_shapes convert_mouse_cursor(ImGuiMouseCursor mc) noexcept {
        #define DEFINE_CASE(x,y) case x: return window::cursor_shapes::y
        switch ( mc ) {
            DEFINE_CASE(ImGuiMouseCursor_Arrow, arrow);
            DEFINE_CASE(ImGuiMouseCursor_TextInput, ibeam);
            DEFINE_CASE(ImGuiMouseCursor_ResizeAll, crosshair);
            DEFINE_CASE(ImGuiMouseCursor_ResizeNS, vresize);
            DEFINE_CASE(ImGuiMouseCursor_ResizeEW, hresize);
            DEFINE_CASE(ImGuiMouseCursor_ResizeNESW, crosshair);
            DEFINE_CASE(ImGuiMouseCursor_ResizeNWSE, crosshair);
            DEFINE_CASE(ImGuiMouseCursor_Hand, hand);
            default:
                E2D_ASSERT_MSG(false, "unexpected imgui mouse cursor");
                return window::cursor_shapes::arrow;
        }
        #undef DEFINE_CASE
    }
}

namespace e2d::imgui
{
    imgui_event_listener::imgui_event_listener(ImGuiIO& io, window& w)
    : io_(io)
    , window_(w) {}

    void imgui_event_listener::on_input_char(char32_t uchar) noexcept {
        if ( uchar <= std::numeric_limits<ImWchar>::max() ) {
            io_.AddInputCharacter(static_cast<ImWchar>(uchar));
        }
    }

    void imgui_event_listener::on_move_cursor(const v2f& pos) noexcept {
        const v2f real_size = window_.real_size().cast_to<f32>();
        if ( math::minimum(real_size) > 0.f ) {
            io_.MousePos = v2f(pos.x, real_size.y - pos.y) / real_size * v2f(io_.DisplaySize);
        }
    }

    void imgui_event_listener::on_mouse_scroll(const v2f& delta) noexcept {
        io_.MouseWheel += delta.y;
        io_.MouseWheelH += delta.x;
    }

    void imgui_event_listener::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        E2D_UNUSED(scancode);
        auto key_i = utils::enum_to_underlying(key);
        if ( key_i < std::size(io_.KeysDown) ) {
            switch ( act ) {
                case keyboard_key_action::press:
                case keyboard_key_action::repeat:
                    io_.KeysDown[key_i] = true;
                    break;
                case keyboard_key_action::release:
                    io_.KeysDown[key_i] = false;
                    break;
                case keyboard_key_action::unknown:
                    break;
            }
        }
    }
}
