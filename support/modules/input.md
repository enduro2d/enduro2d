# Input

## Properties

- `mouse: mouse, RO`
- `keyboard: keyboard, RO`

# Mouse

## Enums

```lua
e2d.mouse_button = {
    left, right, middle, x1, x2
}
```

## Properties

- `cursor_pos: v2f, RO`
- `scroll_delta: v2f, RO`

- `is_any_button_pressed: boolean, RO`
- `is_any_button_just_pressed: boolean, RO`
- `is_any_button_just_released: boolean, RO`

- `pressed_buttons: [mouse_button], RO`
- `just_pressed_buttons: [mouse_button], RO`
- `just_released_buttons: [mouse_button], RO`

## Functions

- `is_button_pressed(button: mouse_button) -> boolean`
- `is_button_just_pressed(button: mouse_button) -> boolean`
- `is_button_just_released(button: mouse_button) -> boolean`

## Example

```lua
local d = e2d.debug
local m = e2d.input.mouse

d:trace("Cursor position: " .. m.cursor_pos)

local left_button = e2d.mouse_button.left
if m:is_button_just_pressed(left_button) then
    d:trace("Mouse just pressed")
end
```

# Keyboard

## Enums

```lua
e2d.keyboard_key = {
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,

    a, b, c, d, e, f, g, h, i, j, k, l, m,
    n, o, p, q, r, s, t, u, v, w, x, y, z,

    f1, f2, f3, f4, f5,
    f6, f7, f8, f9, f10,
    f11, f12, f13, f14, f15,
    f16, f17, f18, f19, f20,
    f21, f22, f23, f24, f25,

    minus, equal, backspace, section_sign, grave_accent,

    lbracket, rbracket, semicolon, apostrophe, backslash,

    comma, period, slash,

    escape, tab, caps_lock, space, enter,

    lshift, rshift, lcontrol, rcontrol,
    lalt, ralt, lsuper, rsuper, menu,

    print_screen, scroll_lock, pause,

    insert, del, home, end, page_up, page_down,

    left, up, right, down,

    kp_0, kp_1, kp_2, kp_3, kp_4,
    kp_5, kp_6, kp_7, kp_8, kp_9,

    kp_num_lock, kp_divide, kp_multiply, kp_subtract,
    kp_add, kp_enter, kp_equal, kp_decimal
}
```

## Properties

- `input_text: string, RO`

- `is_any_key_pressed: boolean, RO`
- `is_any_key_just_pressed: boolean, RO`
- `is_any_key_just_released: boolean, RO`

- `pressed_keys: [keyboard_key], RO`
- `just_pressed_keys: [keyboard_key], RO`
- `just_released_keys: [keyboard_key], RO`

## Functions

- `is_key_pressed(key: keyboard_key) -> boolean`
- `is_key_just_pressed(key: keyboard_key) -> boolean`
- `is_key_just_released(key: keyboard_key) -> boolean`

## Example

```lua
local d = e2d.debug
local k = e2d.input.keyboard

local forward_key = e2d.keyboard_key.w
if k:is_key_pressed(forward_key) then
    d:trace("Move forward")
end
```

