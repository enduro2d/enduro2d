/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../base/_all.hpp"
#include "../math/_all.hpp"
#include "../utils/_all.hpp"

#include <promise.hpp/promise.hpp>
#include <promise.hpp/bonus/jobber.hpp>
#include <promise.hpp/bonus/scheduler.hpp>

namespace e2d
{
    namespace stdex
    {
        using namespace jobber_hpp;
        using namespace promise_hpp;
        using namespace scheduler_hpp;
    }

    class audio;
    class sound_stream;
    class sound_source;
    class dbgui;
    class debug;
    class deferrer;
    class engine;
    class mouse;
    class keyboard;
    class input;
    class platform;
    class render;
    class shader;
    class texture;
    class index_buffer;
    class vertex_buffer;
    class render_target;
    class pixel_declaration;
    class index_declaration;
    class vertex_declaration;
    class vfs;
    class window;
}

namespace e2d
{
    template < typename ModuleT >
    ModuleT& the() {
        return modules::instance<ModuleT>();
    }
}

namespace e2d
{
    ENUM_HPP_CLASS_DECL(mouse_button, u8,
        (left)
        (right)
        (middle)
        (x1)
        (x2)
        (unknown))
    ENUM_HPP_REGISTER_TRAITS(mouse_button)

    ENUM_HPP_CLASS_DECL(keyboard_key, u16,
        (_0) (_1) (_2) (_3) (_4) (_5) (_6) (_7) (_8) (_9)

        (a) (b) (c) (d) (e) (f) (g) (h) (i) (j) (k) (l) (m)
        (n) (o) (p) (q) (r) (s) (t) (u) (v) (w) (x) (y) (z)

        (f1) (f2) (f3) (f4) (f5)
        (f6) (f7) (f8) (f9) (f10)
        (f11) (f12) (f13) (f14) (f15)
        (f16) (f17) (f18) (f19) (f20)
        (f21) (f22) (f23) (f24) (f25)

        (minus) (equal) (backspace) (section_sign) (grave_accent)

        (lbracket) (rbracket) (semicolon) (apostrophe) (backslash)

        (comma) (period) (slash)

        (escape) (tab) (caps_lock) (space) (enter)

        (lshift) (rshift) (lcontrol) (rcontrol)
        (lalt) (ralt) (lsuper) (rsuper) (menu)

        (print_screen) (scroll_lock) (pause)

        (insert) (del) (home) (end) (page_up) (page_down)

        (left) (up) (right) (down)

        (kp_0) (kp_1) (kp_2) (kp_3) (kp_4)
        (kp_5) (kp_6) (kp_7) (kp_8) (kp_9)

        (kp_num_lock) (kp_divide) (kp_multiply) (kp_subtract)
        (kp_add) (kp_enter) (kp_equal) (kp_decimal)

        (unknown))
    ENUM_HPP_REGISTER_TRAITS(keyboard_key)

    ENUM_HPP_CLASS_DECL(mouse_button_action, u8,
        (press)
        (release)
        (unknown))
    ENUM_HPP_REGISTER_TRAITS(mouse_button_action)

    ENUM_HPP_CLASS_DECL(keyboard_key_action, u8,
        (press)
        (repeat)
        (release)
        (unknown))
    ENUM_HPP_REGISTER_TRAITS(keyboard_key_action)
}
