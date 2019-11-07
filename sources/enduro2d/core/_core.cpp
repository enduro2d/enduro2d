/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/_core.hpp>

namespace e2d
{
    const char* mouse_button_to_cstr(mouse_button btn) noexcept {
        #define DEFINE_CASE(x) case mouse_button::x: return #x
        switch ( btn ) {
            DEFINE_CASE(left);
            DEFINE_CASE(right);
            DEFINE_CASE(middle);
            DEFINE_CASE(x1);
            DEFINE_CASE(x2);
            DEFINE_CASE(unknown);
            default:
                E2D_ASSERT_MSG(false, "unexpected mouse button");
                return "";
        }
        #undef DEFINE_CASE
    }

    const char* keyboard_key_to_cstr(keyboard_key key) noexcept {
        #define DEFINE_CASE(x) case keyboard_key::x: return #x
        switch ( key ) {
            DEFINE_CASE(_0);
            DEFINE_CASE(_1);
            DEFINE_CASE(_2);
            DEFINE_CASE(_3);
            DEFINE_CASE(_4);
            DEFINE_CASE(_5);
            DEFINE_CASE(_6);
            DEFINE_CASE(_7);
            DEFINE_CASE(_8);
            DEFINE_CASE(_9);

            DEFINE_CASE(a);
            DEFINE_CASE(b);
            DEFINE_CASE(c);
            DEFINE_CASE(d);
            DEFINE_CASE(e);
            DEFINE_CASE(f);
            DEFINE_CASE(g);
            DEFINE_CASE(h);
            DEFINE_CASE(i);
            DEFINE_CASE(j);
            DEFINE_CASE(k);
            DEFINE_CASE(l);
            DEFINE_CASE(m);
            DEFINE_CASE(n);
            DEFINE_CASE(o);
            DEFINE_CASE(p);
            DEFINE_CASE(q);
            DEFINE_CASE(r);
            DEFINE_CASE(s);
            DEFINE_CASE(t);
            DEFINE_CASE(u);
            DEFINE_CASE(v);
            DEFINE_CASE(w);
            DEFINE_CASE(x);
            DEFINE_CASE(y);
            DEFINE_CASE(z);

            DEFINE_CASE(f1);
            DEFINE_CASE(f2);
            DEFINE_CASE(f3);
            DEFINE_CASE(f4);
            DEFINE_CASE(f5);
            DEFINE_CASE(f6);
            DEFINE_CASE(f7);
            DEFINE_CASE(f8);
            DEFINE_CASE(f9);
            DEFINE_CASE(f10);
            DEFINE_CASE(f11);
            DEFINE_CASE(f12);
            DEFINE_CASE(f13);
            DEFINE_CASE(f14);
            DEFINE_CASE(f15);
            DEFINE_CASE(f16);
            DEFINE_CASE(f17);
            DEFINE_CASE(f18);
            DEFINE_CASE(f19);
            DEFINE_CASE(f20);
            DEFINE_CASE(f21);
            DEFINE_CASE(f22);
            DEFINE_CASE(f23);
            DEFINE_CASE(f24);
            DEFINE_CASE(f25);

            DEFINE_CASE(minus);
            DEFINE_CASE(equal);
            DEFINE_CASE(backspace);
            DEFINE_CASE(section_sign);
            DEFINE_CASE(grave_accent);

            DEFINE_CASE(lbracket);
            DEFINE_CASE(rbracket);
            DEFINE_CASE(semicolon);
            DEFINE_CASE(apostrophe);
            DEFINE_CASE(backslash);

            DEFINE_CASE(comma);
            DEFINE_CASE(period);
            DEFINE_CASE(slash);

            DEFINE_CASE(escape);
            DEFINE_CASE(tab);
            DEFINE_CASE(caps_lock);
            DEFINE_CASE(space);
            DEFINE_CASE(enter);

            DEFINE_CASE(lshift);
            DEFINE_CASE(rshift);
            DEFINE_CASE(lcontrol);
            DEFINE_CASE(rcontrol);
            DEFINE_CASE(lalt);
            DEFINE_CASE(ralt);
            DEFINE_CASE(lsuper);
            DEFINE_CASE(rsuper);
            DEFINE_CASE(menu);

            DEFINE_CASE(print_screen);
            DEFINE_CASE(scroll_lock);
            DEFINE_CASE(pause);

            DEFINE_CASE(insert);
            DEFINE_CASE(del);
            DEFINE_CASE(home);
            DEFINE_CASE(end);
            DEFINE_CASE(page_up);
            DEFINE_CASE(page_down);

            DEFINE_CASE(left);
            DEFINE_CASE(up);
            DEFINE_CASE(right);
            DEFINE_CASE(down);

            DEFINE_CASE(kp_0);
            DEFINE_CASE(kp_1);
            DEFINE_CASE(kp_2);
            DEFINE_CASE(kp_3);
            DEFINE_CASE(kp_4);
            DEFINE_CASE(kp_5);
            DEFINE_CASE(kp_6);
            DEFINE_CASE(kp_7);
            DEFINE_CASE(kp_8);
            DEFINE_CASE(kp_9);

            DEFINE_CASE(kp_num_lock);
            DEFINE_CASE(kp_divide);
            DEFINE_CASE(kp_multiply);
            DEFINE_CASE(kp_subtract);
            DEFINE_CASE(kp_add);
            DEFINE_CASE(kp_enter);
            DEFINE_CASE(kp_equal);
            DEFINE_CASE(kp_decimal);

            DEFINE_CASE(unknown);
            default:
                E2D_ASSERT_MSG(false, "unexpected keyboard key");
                return "";
        }
        #undef DEFINE_CASE
    }

    const char* mouse_button_action_to_cstr(mouse_button_action action) noexcept {
        #define DEFINE_CASE(x) case mouse_button_action::x: return #x
        switch ( action ) {
            DEFINE_CASE(press);
            DEFINE_CASE(release);
            DEFINE_CASE(unknown);
            default:
                E2D_ASSERT_MSG(false, "unexpected mouse button action");
                return "";
        }
        #undef DEFINE_CASE
    }

    const char* keyboard_key_action_to_cstr(keyboard_key_action action) noexcept {
        #define DEFINE_CASE(x) case keyboard_key_action::x: return #x
        switch ( action ) {
            DEFINE_CASE(press);
            DEFINE_CASE(repeat);
            DEFINE_CASE(release);
            DEFINE_CASE(unknown);
            default:
                E2D_ASSERT_MSG(false, "unexpected keyboard key action");
                return "";
        }
        #undef DEFINE_CASE
    }
}

namespace e2d
{
    bool parse_mouse_button(str_view str, mouse_button& btn) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { btn = mouse_button::x; return true; }
        DEFINE_IF(left);
        DEFINE_IF(right);
        DEFINE_IF(middle);
        DEFINE_IF(x1);
        DEFINE_IF(x2);
        DEFINE_IF(unknown);
    #undef DEFINE_IF
        return false;
    }

    bool parse_keyboard_key(str_view str, keyboard_key& key) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { key = keyboard_key::x; return true; }
        DEFINE_IF(_0);
        DEFINE_IF(_1);
        DEFINE_IF(_2);
        DEFINE_IF(_3);
        DEFINE_IF(_4);
        DEFINE_IF(_5);
        DEFINE_IF(_6);
        DEFINE_IF(_7);
        DEFINE_IF(_8);
        DEFINE_IF(_9);

        DEFINE_IF(a);
        DEFINE_IF(b);
        DEFINE_IF(c);
        DEFINE_IF(d);
        DEFINE_IF(e);
        DEFINE_IF(f);
        DEFINE_IF(g);
        DEFINE_IF(h);
        DEFINE_IF(i);
        DEFINE_IF(j);
        DEFINE_IF(k);
        DEFINE_IF(l);
        DEFINE_IF(m);
        DEFINE_IF(n);
        DEFINE_IF(o);
        DEFINE_IF(p);
        DEFINE_IF(q);
        DEFINE_IF(r);
        DEFINE_IF(s);
        DEFINE_IF(t);
        DEFINE_IF(u);
        DEFINE_IF(v);
        DEFINE_IF(w);
        DEFINE_IF(x);
        DEFINE_IF(y);
        DEFINE_IF(z);

        DEFINE_IF(f1);
        DEFINE_IF(f2);
        DEFINE_IF(f3);
        DEFINE_IF(f4);
        DEFINE_IF(f5);
        DEFINE_IF(f6);
        DEFINE_IF(f7);
        DEFINE_IF(f8);
        DEFINE_IF(f9);
        DEFINE_IF(f10);
        DEFINE_IF(f11);
        DEFINE_IF(f12);
        DEFINE_IF(f13);
        DEFINE_IF(f14);
        DEFINE_IF(f15);
        DEFINE_IF(f16);
        DEFINE_IF(f17);
        DEFINE_IF(f18);
        DEFINE_IF(f19);
        DEFINE_IF(f20);
        DEFINE_IF(f21);
        DEFINE_IF(f22);
        DEFINE_IF(f23);
        DEFINE_IF(f24);
        DEFINE_IF(f25);

        DEFINE_IF(minus);
        DEFINE_IF(equal);
        DEFINE_IF(backspace);
        DEFINE_IF(section_sign);
        DEFINE_IF(grave_accent);

        DEFINE_IF(lbracket);
        DEFINE_IF(rbracket);
        DEFINE_IF(semicolon);
        DEFINE_IF(apostrophe);
        DEFINE_IF(backslash);

        DEFINE_IF(comma);
        DEFINE_IF(period);
        DEFINE_IF(slash);

        DEFINE_IF(escape);
        DEFINE_IF(tab);
        DEFINE_IF(caps_lock);
        DEFINE_IF(space);
        DEFINE_IF(enter);

        DEFINE_IF(lshift);
        DEFINE_IF(rshift);
        DEFINE_IF(lcontrol);
        DEFINE_IF(rcontrol);
        DEFINE_IF(lalt);
        DEFINE_IF(ralt);
        DEFINE_IF(lsuper);
        DEFINE_IF(rsuper);
        DEFINE_IF(menu);

        DEFINE_IF(print_screen);
        DEFINE_IF(scroll_lock);
        DEFINE_IF(pause);

        DEFINE_IF(insert);
        DEFINE_IF(del);
        DEFINE_IF(home);
        DEFINE_IF(end);
        DEFINE_IF(page_up);
        DEFINE_IF(page_down);

        DEFINE_IF(left);
        DEFINE_IF(up);
        DEFINE_IF(right);
        DEFINE_IF(down);

        DEFINE_IF(kp_0);
        DEFINE_IF(kp_1);
        DEFINE_IF(kp_2);
        DEFINE_IF(kp_3);
        DEFINE_IF(kp_4);
        DEFINE_IF(kp_5);
        DEFINE_IF(kp_6);
        DEFINE_IF(kp_7);
        DEFINE_IF(kp_8);
        DEFINE_IF(kp_9);

        DEFINE_IF(kp_num_lock);
        DEFINE_IF(kp_divide);
        DEFINE_IF(kp_multiply);
        DEFINE_IF(kp_subtract);
        DEFINE_IF(kp_add);
        DEFINE_IF(kp_enter);
        DEFINE_IF(kp_equal);
        DEFINE_IF(kp_decimal);

        DEFINE_IF(unknown);
    #undef DEFINE_IF
        return false;
    }

    bool parse_mouse_button_action(str_view str, mouse_button_action& act) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { act = mouse_button_action::x; return true; }
        DEFINE_IF(press);
        DEFINE_IF(release);
        DEFINE_IF(unknown);
    #undef DEFINE_IF
        return false;
    }

    bool parse_keyboard_key_action(str_view str, keyboard_key_action& act) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { act = keyboard_key_action::x; return true; }
        DEFINE_IF(press);
        DEFINE_IF(repeat);
        DEFINE_IF(release);
        DEFINE_IF(unknown);
    #undef DEFINE_IF
        return false;
    }
}
