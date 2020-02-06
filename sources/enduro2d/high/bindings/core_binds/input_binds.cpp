/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core_binds.hpp"

namespace
{
    using namespace e2d;

    vector<str> convert_keys_to_strings(const vector<keyboard_key>& keys) {
        vector<str> strings;
        strings.reserve(keys.size());
        for ( keyboard_key key : keys ) {
            strings.push_back(str(enum_hpp::to_string_or_throw(key)));
        }
        return strings;
    }

    vector<str> convert_buttons_to_strings(const vector<mouse_button>& buttons) {
        vector<str> strings;
        strings.reserve(buttons.size());
        for ( mouse_button btn : buttons ) {
            strings.push_back(str(enum_hpp::to_string_or_throw(btn)));
        }
        return strings;
    }
}

namespace e2d::bindings::core
{
    void bind_input(sol::state& l) {
        l.new_usertype<input>("input",
            sol::no_constructor,

            "mouse", sol::property([](const input& i) -> const mouse& {
                return i.mouse();
            }),

            "keyboard", sol::property([](const input& i) -> const keyboard& {
                return i.keyboard();
            })
        );

        l.new_usertype<mouse>("mouse",
            sol::no_constructor,

            "cursor_pos", sol::property([](const mouse& m) -> v2f {
                return m.cursor_pos();
            }),

            "scroll_delta", sol::property([](const mouse& m) -> v2f {
                return m.scroll_delta();
            }),

            "is_any_button_pressed", sol::property([](const mouse& m) -> bool {
                return m.is_any_button_pressed();
            }),

            "is_any_button_just_pressed", sol::property([](const mouse& m) -> bool {
                return m.is_any_button_just_pressed();
            }),

            "is_any_button_just_released", sol::property([](const mouse& m) -> bool {
                return m.is_any_button_just_released();
            }),

            "is_button_pressed", [](const mouse& m, str_view n) -> bool {
                if ( auto btn = enum_hpp::from_string<mouse_button>(n) ) {
                    return m.is_button_pressed(*btn);
                }
                the<debug>().error("MOUSE: unknown button name:\n"
                    "--> Function: `is_button_pressed`\n"
                    "--> Button: %0",
                    n);
                return false;
            },

            "is_button_just_pressed", [](const mouse& m, str_view n) -> bool {
                if ( auto btn = enum_hpp::from_string<mouse_button>(n) ) {
                    return m.is_button_just_pressed(*btn);
                }
                the<debug>().error("MOUSE: unknown button name:\n"
                    "--> Function: `is_button_just_pressed`\n"
                    "--> Button: %0",
                    n);
                return false;
            },

            "is_button_just_released", [](const mouse& m, str_view n) -> bool {
                if ( auto btn = enum_hpp::from_string<mouse_button>(n) ) {
                    return m.is_button_just_released(*btn);
                }
                the<debug>().error("MOUSE: unknown button name:\n"
                    "--> Function: `is_button_just_released`\n"
                    "--> Button: %0",
                    n);
                return false;
            },

            "pressed_buttons", sol::property([](const mouse& m) -> vector<str> {
                return convert_buttons_to_strings(m.pressed_buttons());
            }),

            "just_pressed_buttons", sol::property([](const mouse& m) -> vector<str> {
                return convert_buttons_to_strings(m.just_pressed_buttons());
            }),

            "just_released_buttons", sol::property([](const mouse& m) -> vector<str> {
                return convert_buttons_to_strings(m.just_released_buttons());
            })
        );

        l.new_usertype<keyboard>("keyboard",
            sol::no_constructor,

            "input_text", sol::property([](const keyboard& k) -> str {
                return make_utf8(k.input_text());
            }),

            "is_any_key_pressed", sol::property([](const keyboard& k) -> bool {
                return k.is_any_key_pressed();
            }),

            "is_any_key_just_pressed", sol::property([](const keyboard& k) -> bool {
                return k.is_any_key_just_pressed();
            }),

            "is_any_key_just_released", sol::property([](const keyboard& k) -> bool {
                return k.is_any_key_just_released();
            }),

            "is_key_pressed", [](const keyboard& k, str_view n) -> bool {
                if ( auto key = enum_hpp::from_string<keyboard_key>(n) ) {
                    return k.is_key_pressed(*key);
                }
                the<debug>().error("KEYBOARD: unknown key name:\n"
                    "--> Function: `is_key_pressed`\n"
                    "--> Key: %0",
                    n);
                return false;
            },

            "is_key_just_pressed", [](const keyboard& k, str_view n) -> bool {
                if ( auto key = enum_hpp::from_string<keyboard_key>(n) ) {
                    return k.is_key_just_pressed(*key);
                }
                the<debug>().error("KEYBOARD: unknown key name:\n"
                    "--> Function: `is_key_just_pressed`\n"
                    "--> Key: %0",
                    n);
                return false;
            },

            "is_key_just_released", [](const keyboard& k, str_view n) -> bool {
                if ( auto key = enum_hpp::from_string<keyboard_key>(n) ) {
                    return k.is_key_just_released(*key);
                }
                the<debug>().error("KEYBOARD: unknown key name:\n"
                    "--> Function: `is_key_just_released`\n"
                    "--> Key: %0",
                    n);
                return false;
            },

            "pressed_keys", sol::property([](const keyboard& k) -> vector<str> {
                return convert_keys_to_strings(k.pressed_keys());
            }),

            "just_pressed_keys", sol::property([](const keyboard& k) -> vector<str> {
                return convert_keys_to_strings(k.just_pressed_keys());
            }),

            "just_released_keys", sol::property([](const keyboard& k) -> vector<str> {
                return convert_keys_to_strings(k.just_released_keys());
            })
        );
    }
}
