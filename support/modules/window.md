# WINDOW

## FUNCTIONS

- `hide(): void`
- `show(): void`
- `restore(): void`
- `minimize(): void`

## PROPERTIES

- `enabled: boolean` (read_only)
- `visible: boolean` (read_only)
- `focused: boolean` (read_only)
- `minimized: boolean` (read_only)

- `fullscreen: boolean` (read/write)
- `cursor_hidden: boolean` (read/write)

- `real_size: v2f` (read_only)
- `virtual_size: v2f` (read_only)
- `framebuffer_size: v2f` (read_only)

- `title: string` (read/write)
- `should_close: boolean` (read/write)

## EXAMPLE

```lua
local close_window =
    the_keyboard:is_key_just_pressed("f12")

local toggle_fullscreen =
    the_keyboard:is_key_pressed("lsuper") and
    the_keyboard:is_key_just_released("enter")

if close_window then
    the_window.should_close = true
elseif toggle_fullscreen then
    the_window.fullscreen = not the_window.fullscreen
end
```
