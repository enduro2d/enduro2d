# Window

## Properties

- `enabled: boolean, RO`
- `visible: boolean, RO`
- `focused: boolean, RO`
- `minimized: boolean, RO`

- `fullscreen: boolean, RW`
- `cursor_hidden: boolean, RW`

- `real_size: v2f, RO`
- `virtual_size: v2f, RO`
- `framebuffer_size: v2f, RO`

- `title: string, RW`
- `should_close: boolean, RW`

## Functions

- `hide() -> void`
- `show() -> void`
- `restore() -> void`
- `minimize() -> void`

## Example

```lua
local w = e2d.window
local k = e2d.input.keyboard

local close_window =
    k:is_key_just_pressed(e2d.keyboard_key.f32)

local toggle_fullscreen =
    k:is_key_pressed(e2d.keyboard_key.lsuper) and
    k:is_key_just_released(e2d.keyboard_key.enter)

if close_window then
    w.should_close = true
elseif toggle_fullscreen then
    w.fullscreen = not w.fullscreen
end
```

