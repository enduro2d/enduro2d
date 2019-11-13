/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace
{
    using namespace e2d;

    vector<str> convert_keys_to_strings(const vector<keyboard_key>& keys) {
        vector<str> strings;
        strings.reserve(keys.size());
        std::transform(
            keys.begin(),
            keys.end(),
            std::back_inserter(strings),
            &keyboard_key_to_cstr);
        return strings;
    }

    vector<str> convert_buttons_to_strings(const vector<mouse_button>& buttons) {
        vector<str> strings;
        strings.reserve(buttons.size());
        std::transform(
            buttons.begin(),
            buttons.end(),
            std::back_inserter(strings),
            &mouse_button_to_cstr);
        return strings;
    }
}

namespace e2d::bindings::core
{
    void bind_input(sol::state& l) {
        l.new_usertype<input>("input",
            sol::no_constructor,

            "mouse", sol::property(&input::mouse),
            "keyboard", sol::property(&input::keyboard)
        );

        l.new_usertype<mouse>("mouse",
            sol::no_constructor,

            "cursor_pos", sol::property([](const mouse& m){
                return m.cursor_pos();
            }),

            "scroll_delta", sol::property([](const mouse& m){
                return m.scroll_delta();
            }),

            "is_any_button_pressed", sol::property(&mouse::is_any_button_pressed),
            "is_any_button_just_pressed", sol::property(&mouse::is_any_button_just_pressed),
            "is_any_button_just_released", sol::property(&mouse::is_any_button_just_released),

            "is_button_pressed", [](const mouse& m, const char* n){
                mouse_button btn = mouse_button::unknown;
                if ( parse_mouse_button(n, btn) ) {
                    return m.is_button_pressed(btn);
                }
                the<debug>().error("MOUSE: unknown button name:\n"
                    "--> Function: `is_button_pressed`\n"
                    "--> Button: %0",
                    n);
                return false;
            },

            "is_button_just_pressed", [](const mouse& m, const char* n){
                mouse_button btn = mouse_button::unknown;
                if ( parse_mouse_button(n, btn) ) {
                    return m.is_button_just_pressed(btn);
                }
                the<debug>().error("MOUSE: unknown button name:\n"
                    "--> Function: `is_button_just_pressed`\n"
                    "--> Button: %0",
                    n);
                return false;
            },

            "is_button_just_released", [](const mouse& m, const char* n){
                mouse_button btn = mouse_button::unknown;
                if ( parse_mouse_button(n, btn) ) {
                    return m.is_button_just_released(btn);
                }
                the<debug>().error("MOUSE: unknown button name:\n"
                    "--> Function: `is_button_just_released`\n"
                    "--> Button: %0",
                    n);
                return false;
            },

            "pressed_buttons", sol::property([](const mouse& m){
                return convert_buttons_to_strings(m.pressed_buttons());
            }),

            "just_pressed_buttons", sol::property([](const mouse& m){
                return convert_buttons_to_strings(m.just_pressed_buttons());
            }),

            "just_released_buttons", sol::property([](const mouse& m){
                return convert_buttons_to_strings(m.just_released_buttons());
            })
        );

        l.new_usertype<keyboard>("keyboard",
            sol::no_constructor,

            "input_text", sol::property([](const keyboard& k){
                return make_utf8(k.input_text());
            }),

            "is_any_key_pressed", sol::property(&keyboard::is_any_key_pressed),
            "is_any_key_just_pressed", sol::property(&keyboard::is_any_key_just_pressed),
            "is_any_key_just_released", sol::property(&keyboard::is_any_key_just_released),

            "is_key_pressed", [](const keyboard& k, const char* n){
                keyboard_key key = keyboard_key::unknown;
                if ( parse_keyboard_key(n, key) ) {
                    return k.is_key_pressed(key);
                }
                the<debug>().error("KEYBOARD: unknown key name:\n"
                    "--> Function: `is_key_pressed`\n"
                    "--> Key: %0",
                    n);
                return false;
            },

            "is_key_just_pressed", [](const keyboard& k, const char* n){
                keyboard_key key = keyboard_key::unknown;
                if ( parse_keyboard_key(n, key) ) {
                    return k.is_key_just_pressed(key);
                }
                the<debug>().error("KEYBOARD: unknown key name:\n"
                    "--> Function: `is_key_just_pressed`\n"
                    "--> Key: %0",
                    n);
                return false;
            },

            "is_key_just_released", [](const keyboard& k, const char* n){
                keyboard_key key = keyboard_key::unknown;
                if ( parse_keyboard_key(n, key) ) {
                    return k.is_key_just_released(key);
                }
                the<debug>().error("KEYBOARD: unknown key name:\n"
                    "--> Function: `is_key_just_released`\n"
                    "--> Key: %0",
                    n);
                return false;
            },

            "pressed_keys", sol::property([](const keyboard& k){
                return convert_keys_to_strings(k.pressed_keys());
            }),

            "just_pressed_keys", sol::property([](const keyboard& k){
                return convert_keys_to_strings(k.just_pressed_keys());
            }),

            "just_released_keys", sol::property([](const keyboard& k){
                return convert_keys_to_strings(k.just_released_keys());
            })
        );
    }
}
