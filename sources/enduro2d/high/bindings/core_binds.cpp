/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bindings.hpp"

namespace
{
    using namespace e2d;

    void bind_debug(sol::state& l) {
        l.new_usertype<debug>("debug",
            "min_level", sol::property(&debug::min_level, &debug::set_min_level),
            "log", [](debug& d, debug::level l, const char* s){ d.log(l, s); },
            "trace", [](debug& d, const char* s){ d.trace(s); },
            "warning", [](debug& d, const char* s){ d.warning(s); },
            "error", [](debug& d, const char* s){ d.error(s); },
            "fatal", [](debug& d, const char* s){ d.fatal(s); }
        );
    #define DEBUG_LEVEL_PAIR(x) {#x, debug::level::x},
        l["debug"].get_or_create<sol::table>()
        .new_enum<debug::level>("level", {
            DEBUG_LEVEL_PAIR(trace)
            DEBUG_LEVEL_PAIR(warning)
            DEBUG_LEVEL_PAIR(error)
            DEBUG_LEVEL_PAIR(fatal)
        });
    #undef DEBUG_LEVEL_PAIR
    }

    void bind_engine(sol::state& l) {
        l.new_usertype<engine>("engine",
            "time", sol::property(&engine::time),
            "delta_time", sol::property(&engine::delta_time),
            "frame_rate", sol::property(&engine::frame_rate),
            "frame_count", sol::property(&engine::frame_count),
            "realtime_time", sol::property(&engine::realtime_time)
        );
    }

    void bind_input(sol::state& l) {
        l.new_usertype<input>("input",
            "mouse", sol::property(&input::mouse),
            "keyboard", sol::property(&input::keyboard)
        );

        l.new_usertype<mouse>("mouse",
            "cursor_pos", sol::property(&mouse::cursor_pos),
            "scroll_delta", sol::property(&mouse::scroll_delta),

            "is_any_button_pressed", sol::property(&mouse::is_any_button_pressed),
            "is_any_button_just_pressed", sol::property(&mouse::is_any_button_just_pressed),
            "is_any_button_just_released", sol::property(&mouse::is_any_button_just_released),

            "is_button_pressed", &mouse::is_button_pressed,
            "is_button_just_pressed", &mouse::is_button_just_pressed,
            "is_button_just_released", &mouse::is_button_just_released,

            "pressed_buttons", sol::property(&mouse::pressed_buttons),
            "just_pressed_buttons", sol::property(&mouse::pressed_buttons),
            "just_released_buttons", sol::property(&mouse::just_released_buttons)
        );

        l.new_usertype<keyboard>("keyboard",
            "input_text", sol::property([](const keyboard& k){ return make_utf8(k.input_text()); }),

            "is_any_key_pressed", sol::property(&keyboard::is_any_key_pressed),
            "is_any_key_just_pressed", sol::property(&keyboard::is_any_key_just_pressed),
            "is_any_key_just_released", sol::property(&keyboard::is_any_key_just_released),

            "is_key_pressed", &keyboard::is_key_pressed,
            "is_key_just_pressed", &keyboard::is_key_just_pressed,
            "is_key_just_released", &keyboard::is_key_just_released,

            "pressed_keys", sol::property(&keyboard::pressed_keys),
            "just_pressed_keys", sol::property(&keyboard::pressed_keys),
            "just_released_keys", sol::property(&keyboard::just_released_keys)
        );

    #define MOUSE_BUTTON_PAIR(x) {#x, mouse_button::x},
        l.new_enum<mouse_button>("mouse_button", {
            MOUSE_BUTTON_PAIR(left)
            MOUSE_BUTTON_PAIR(right)
            MOUSE_BUTTON_PAIR(middle)
            MOUSE_BUTTON_PAIR(x1)
            MOUSE_BUTTON_PAIR(x2)
        });
    #undef MOUSE_BUTTON_PAIR

    #define KEYBOARD_KEY_PAIR(x) {#x, keyboard_key::x},
        l.new_enum<keyboard_key>("keyboard_key", {
            KEYBOARD_KEY_PAIR(_0)
            KEYBOARD_KEY_PAIR(_1)
            KEYBOARD_KEY_PAIR(_2)
            KEYBOARD_KEY_PAIR(_3)
            KEYBOARD_KEY_PAIR(_4)
            KEYBOARD_KEY_PAIR(_5)
            KEYBOARD_KEY_PAIR(_6)
            KEYBOARD_KEY_PAIR(_7)
            KEYBOARD_KEY_PAIR(_8)
            KEYBOARD_KEY_PAIR(_9)

            KEYBOARD_KEY_PAIR(a)
            KEYBOARD_KEY_PAIR(b)
            KEYBOARD_KEY_PAIR(c)
            KEYBOARD_KEY_PAIR(d)
            KEYBOARD_KEY_PAIR(e)
            KEYBOARD_KEY_PAIR(f)
            KEYBOARD_KEY_PAIR(g)
            KEYBOARD_KEY_PAIR(h)
            KEYBOARD_KEY_PAIR(i)
            KEYBOARD_KEY_PAIR(j)
            KEYBOARD_KEY_PAIR(k)
            KEYBOARD_KEY_PAIR(l)
            KEYBOARD_KEY_PAIR(m)
            KEYBOARD_KEY_PAIR(n)
            KEYBOARD_KEY_PAIR(o)
            KEYBOARD_KEY_PAIR(p)
            KEYBOARD_KEY_PAIR(q)
            KEYBOARD_KEY_PAIR(r)
            KEYBOARD_KEY_PAIR(s)
            KEYBOARD_KEY_PAIR(t)
            KEYBOARD_KEY_PAIR(u)
            KEYBOARD_KEY_PAIR(v)
            KEYBOARD_KEY_PAIR(w)
            KEYBOARD_KEY_PAIR(x)
            KEYBOARD_KEY_PAIR(y)
            KEYBOARD_KEY_PAIR(z)

            KEYBOARD_KEY_PAIR(f1)
            KEYBOARD_KEY_PAIR(f2)
            KEYBOARD_KEY_PAIR(f3)
            KEYBOARD_KEY_PAIR(f4)
            KEYBOARD_KEY_PAIR(f5)
            KEYBOARD_KEY_PAIR(f6)
            KEYBOARD_KEY_PAIR(f7)
            KEYBOARD_KEY_PAIR(f8)
            KEYBOARD_KEY_PAIR(f9)
            KEYBOARD_KEY_PAIR(f10)
            KEYBOARD_KEY_PAIR(f11)
            KEYBOARD_KEY_PAIR(f12)
            KEYBOARD_KEY_PAIR(f13)
            KEYBOARD_KEY_PAIR(f14)
            KEYBOARD_KEY_PAIR(f15)
            KEYBOARD_KEY_PAIR(f16)
            KEYBOARD_KEY_PAIR(f17)
            KEYBOARD_KEY_PAIR(f18)
            KEYBOARD_KEY_PAIR(f19)
            KEYBOARD_KEY_PAIR(f20)
            KEYBOARD_KEY_PAIR(f21)
            KEYBOARD_KEY_PAIR(f22)
            KEYBOARD_KEY_PAIR(f23)
            KEYBOARD_KEY_PAIR(f24)
            KEYBOARD_KEY_PAIR(f25)

            KEYBOARD_KEY_PAIR(minus)
            KEYBOARD_KEY_PAIR(equal)
            KEYBOARD_KEY_PAIR(backspace)
            KEYBOARD_KEY_PAIR(section_sign)
            KEYBOARD_KEY_PAIR(grave_accent)

            KEYBOARD_KEY_PAIR(lbracket)
            KEYBOARD_KEY_PAIR(rbracket)
            KEYBOARD_KEY_PAIR(semicolon)
            KEYBOARD_KEY_PAIR(apostrophe)
            KEYBOARD_KEY_PAIR(backslash)

            KEYBOARD_KEY_PAIR(comma)
            KEYBOARD_KEY_PAIR(period)
            KEYBOARD_KEY_PAIR(slash)

            KEYBOARD_KEY_PAIR(escape)
            KEYBOARD_KEY_PAIR(tab)
            KEYBOARD_KEY_PAIR(caps_lock)
            KEYBOARD_KEY_PAIR(space)
            KEYBOARD_KEY_PAIR(enter)

            KEYBOARD_KEY_PAIR(lshift)
            KEYBOARD_KEY_PAIR(rshift)
            KEYBOARD_KEY_PAIR(lcontrol)
            KEYBOARD_KEY_PAIR(rcontrol)
            KEYBOARD_KEY_PAIR(lalt)
            KEYBOARD_KEY_PAIR(ralt)
            KEYBOARD_KEY_PAIR(lsuper)
            KEYBOARD_KEY_PAIR(rsuper)
            KEYBOARD_KEY_PAIR(menu)

            KEYBOARD_KEY_PAIR(print_screen)
            KEYBOARD_KEY_PAIR(scroll_lock)
            KEYBOARD_KEY_PAIR(pause)

            KEYBOARD_KEY_PAIR(insert)
            KEYBOARD_KEY_PAIR(del)
            KEYBOARD_KEY_PAIR(home)
            KEYBOARD_KEY_PAIR(end)
            KEYBOARD_KEY_PAIR(page_up)
            KEYBOARD_KEY_PAIR(page_down)

            KEYBOARD_KEY_PAIR(left)
            KEYBOARD_KEY_PAIR(up)
            KEYBOARD_KEY_PAIR(right)
            KEYBOARD_KEY_PAIR(down)

            KEYBOARD_KEY_PAIR(kp_0)
            KEYBOARD_KEY_PAIR(kp_1)
            KEYBOARD_KEY_PAIR(kp_2)
            KEYBOARD_KEY_PAIR(kp_3)
            KEYBOARD_KEY_PAIR(kp_4)
            KEYBOARD_KEY_PAIR(kp_5)
            KEYBOARD_KEY_PAIR(kp_6)
            KEYBOARD_KEY_PAIR(kp_7)
            KEYBOARD_KEY_PAIR(kp_8)
            KEYBOARD_KEY_PAIR(kp_9)

            KEYBOARD_KEY_PAIR(kp_num_lock)
            KEYBOARD_KEY_PAIR(kp_divide)
            KEYBOARD_KEY_PAIR(kp_multiply)
            KEYBOARD_KEY_PAIR(kp_subtract)
            KEYBOARD_KEY_PAIR(kp_add)
            KEYBOARD_KEY_PAIR(kp_enter)
            KEYBOARD_KEY_PAIR(kp_equal)
            KEYBOARD_KEY_PAIR(kp_decimal)
        });
    #undef KEYBOARD_KEY_PAIR
    }

    void bind_window(sol::state& l) {
        l.new_usertype<window>("window",
            "hide", &window::hide,
            "show", &window::show,
            "restore", &window::restore,
            "minimize", &window::minimize,

            "enabled", sol::property(&window::enabled),
            "visible", sol::property(&window::visible),
            "focused", sol::property(&window::focused),
            "minimized", sol::property(&window::minimized),

            "fullscreen", sol::property(
                &window::fullscreen,
                &window::toggle_fullscreen),

            "cursor_hidden", sol::property(
                &window::is_cursor_hidden,
                [](window& w, bool yesno){
                    if ( yesno ) {
                        w.hide_cursor();
                    } else {
                        w.show_cursor();
                    }
                }),

            "real_size", sol::property([](const window& w){ return w.real_size().cast_to<f32>(); }),
            "virtual_size", sol::property([](const window& w){ return w.virtual_size().cast_to<f32>(); }),
            "framebuffer_size", sol::property([](const window& w){ return w.framebuffer_size().cast_to<f32>(); }),

            "title", sol::property(&window::title, &window::set_title),
            "should_close", sol::property(&window::should_close, &window::set_should_close)
        );
    }
}

namespace e2d::bindings
{
    void bind_core(sol::state& l) {
        bind_debug(l);
        bind_engine(l);
        bind_input(l);
        bind_window(l);
    }
}
