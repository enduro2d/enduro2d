/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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
