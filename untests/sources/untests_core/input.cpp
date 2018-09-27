/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_core.hpp"
using namespace e2d;

TEST_CASE("input"){
    SECTION("mouse"){
        {
            input i;
            const mouse& m = i.mouse();

            REQUIRE(math::approximately(m.cursor_pos(), v2f::zero()));
            REQUIRE(math::approximately(m.scroll_delta(), v2f::zero()));

            i.post_event(input::move_cursor_event{v2f(10.f,20.f)});
            i.post_event(input::mouse_scroll_event{v2f(0.1f,0.2f)});
            REQUIRE(math::approximately(m.cursor_pos(), v2f(10.f,20.f)));
            REQUIRE(math::approximately(m.scroll_delta(), v2f(0.1f,0.2f)));

            i.frame_tick();

            REQUIRE(math::approximately(m.cursor_pos(), v2f(10.f,20.f)));
            REQUIRE(math::approximately(m.scroll_delta(), v2f::zero()));

            i.post_event(input::move_cursor_event{v2f(12.f,22.f)});
            REQUIRE(math::approximately(m.cursor_pos(), v2f(12.f,22.f)));
        }
        {
            input i;
            const mouse& m = i.mouse();

            REQUIRE_FALSE(m.is_any_button_pressed());
            REQUIRE_FALSE(m.is_any_button_just_pressed());
            REQUIRE_FALSE(m.is_any_button_just_released());
            REQUIRE_FALSE(m.is_button_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_released(mouse_button::left));

            i.post_event(input::mouse_button_event{mouse_button::left, mouse_button_action::press});

            REQUIRE(m.is_any_button_pressed());
            REQUIRE(m.is_any_button_just_pressed());
            REQUIRE_FALSE(m.is_any_button_just_released());
            REQUIRE(m.is_button_pressed(mouse_button::left));
            REQUIRE(m.is_button_just_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_released(mouse_button::left));

            i.frame_tick();

            REQUIRE(m.is_any_button_pressed());
            REQUIRE_FALSE(m.is_any_button_just_pressed());
            REQUIRE_FALSE(m.is_any_button_just_released());
            REQUIRE(m.is_button_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_released(mouse_button::left));

            i.post_event(input::mouse_button_event{mouse_button::left, mouse_button_action::release});

            REQUIRE_FALSE(m.is_any_button_pressed());
            REQUIRE_FALSE(m.is_any_button_just_pressed());
            REQUIRE(m.is_any_button_just_released());
            REQUIRE_FALSE(m.is_button_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_pressed(mouse_button::left));
            REQUIRE(m.is_button_just_released(mouse_button::left));

            i.frame_tick();

            REQUIRE_FALSE(m.is_any_button_pressed());
            REQUIRE_FALSE(m.is_any_button_just_pressed());
            REQUIRE_FALSE(m.is_any_button_just_released());
            REQUIRE_FALSE(m.is_button_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_pressed(mouse_button::left));
            REQUIRE_FALSE(m.is_button_just_released(mouse_button::left));
        }
        {
            input i;
            const mouse& m = i.mouse();

            std::vector<mouse_button> buttons;
            m.extract_pressed_buttons(buttons);
            m.extract_just_pressed_buttons(buttons);
            m.extract_just_released_buttons(buttons);
            REQUIRE(buttons.size() == 0);

            buttons.clear();
            i.post_event(input::mouse_button_event{mouse_button::right, mouse_button_action::press});

            m.extract_pressed_buttons(buttons);
            REQUIRE(buttons.size() == 1);
            REQUIRE(buttons[0] == mouse_button::right);

            m.extract_just_pressed_buttons(buttons);
            REQUIRE(buttons.size() == 2);
            REQUIRE(buttons[0] == mouse_button::right);
            REQUIRE(buttons[1] == mouse_button::right);

            m.extract_just_released_buttons(buttons);
            REQUIRE(buttons.size() == 2);
            REQUIRE(buttons[0] == mouse_button::right);
            REQUIRE(buttons[1] == mouse_button::right);

            buttons.clear();
            i.frame_tick();

            m.extract_pressed_buttons(buttons);
            REQUIRE(buttons.size() == 1);
            REQUIRE(buttons[0] == mouse_button::right);

            m.extract_just_pressed_buttons(buttons);
            REQUIRE(buttons.size() == 1);
            REQUIRE(buttons[0] == mouse_button::right);

            m.extract_just_released_buttons(buttons);
            REQUIRE(buttons.size() == 1);
            REQUIRE(buttons[0] == mouse_button::right);

            buttons.clear();
            i.post_event(input::mouse_button_event{mouse_button::right, mouse_button_action::release});

            m.extract_pressed_buttons(buttons);
            REQUIRE(buttons.size() == 0);

            m.extract_just_pressed_buttons(buttons);
            REQUIRE(buttons.size() == 0);

            m.extract_just_released_buttons(buttons);
            REQUIRE(buttons.size() == 1);
            REQUIRE(buttons[0] == mouse_button::right);
        }
    }
    SECTION("keyboard"){
        {
            input i;
            const keyboard& k = i.keyboard();

            REQUIRE(k.input_text() == make_utf32(""));

            i.post_event(input::input_char_event{'H'});
            i.post_event(input::input_char_event{'e'});

            REQUIRE(k.input_text() == make_utf32("He"));

            i.frame_tick();

            REQUIRE(k.input_text() == make_utf32(""));
        }
        {
            input i;
            const keyboard& k = i.keyboard();

            REQUIRE_FALSE(k.is_any_key_pressed());
            REQUIRE_FALSE(k.is_any_key_just_pressed());
            REQUIRE_FALSE(k.is_any_key_just_released());
            REQUIRE_FALSE(k.is_key_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_released(keyboard_key::enter));

            i.post_event(input::keyboard_key_event{keyboard_key::enter, keyboard_key_action::press});

            REQUIRE(k.is_any_key_pressed());
            REQUIRE(k.is_any_key_just_pressed());
            REQUIRE_FALSE(k.is_any_key_just_released());
            REQUIRE(k.is_key_pressed(keyboard_key::enter));
            REQUIRE(k.is_key_just_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_released(keyboard_key::enter));

            i.frame_tick();

            REQUIRE(k.is_any_key_pressed());
            REQUIRE_FALSE(k.is_any_key_just_pressed());
            REQUIRE_FALSE(k.is_any_key_just_released());
            REQUIRE(k.is_key_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_released(keyboard_key::enter));

            i.post_event(input::keyboard_key_event{keyboard_key::enter, keyboard_key_action::repeat});

            REQUIRE(k.is_any_key_pressed());
            REQUIRE_FALSE(k.is_any_key_just_pressed());
            REQUIRE_FALSE(k.is_any_key_just_released());
            REQUIRE(k.is_key_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_released(keyboard_key::enter));

            i.post_event(input::keyboard_key_event{keyboard_key::enter, keyboard_key_action::release});

            REQUIRE_FALSE(k.is_any_key_pressed());
            REQUIRE_FALSE(k.is_any_key_just_pressed());
            REQUIRE(k.is_any_key_just_released());
            REQUIRE_FALSE(k.is_key_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_pressed(keyboard_key::enter));
            REQUIRE(k.is_key_just_released(keyboard_key::enter));

            i.frame_tick();

            REQUIRE_FALSE(k.is_any_key_pressed());
            REQUIRE_FALSE(k.is_any_key_just_pressed());
            REQUIRE_FALSE(k.is_any_key_just_released());
            REQUIRE_FALSE(k.is_key_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_pressed(keyboard_key::enter));
            REQUIRE_FALSE(k.is_key_just_released(keyboard_key::enter));
        }
        {
            input i;
            const keyboard& k = i.keyboard();

            std::vector<keyboard_key> keys;
            k.extract_pressed_keys(keys);
            k.extract_just_pressed_keys(keys);
            k.extract_just_released_keys(keys);
            REQUIRE(keys.size() == 0);

            keys.clear();
            i.post_event(input::keyboard_key_event{keyboard_key::escape, keyboard_key_action::press});

            k.extract_pressed_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            k.extract_just_pressed_keys(keys);
            REQUIRE(keys.size() == 2);
            REQUIRE(keys[0] == keyboard_key::escape);
            REQUIRE(keys[1] == keyboard_key::escape);

            k.extract_just_released_keys(keys);
            REQUIRE(keys.size() == 2);
            REQUIRE(keys[0] == keyboard_key::escape);
            REQUIRE(keys[1] == keyboard_key::escape);

            keys.clear();
            i.frame_tick();

            k.extract_pressed_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            k.extract_just_pressed_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            k.extract_just_released_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            keys.clear();
            i.post_event(input::keyboard_key_event{keyboard_key::escape, keyboard_key_action::repeat});

            k.extract_pressed_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            k.extract_just_pressed_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            k.extract_just_released_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);

            keys.clear();
            i.post_event(input::keyboard_key_event{keyboard_key::escape, keyboard_key_action::release});

            k.extract_pressed_keys(keys);
            REQUIRE(keys.size() == 0);

            k.extract_just_pressed_keys(keys);
            REQUIRE(keys.size() == 0);

            k.extract_just_released_keys(keys);
            REQUIRE(keys.size() == 1);
            REQUIRE(keys[0] == keyboard_key::escape);
        }
    }
}
