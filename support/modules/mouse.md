# MOUSE

## FUNCTIONS

- `is_button_pressed(button: string): boolean`
- `is_button_just_pressed(button: string): boolean`
- `is_button_just_released(button: string): boolean`

## PROPERTIES

- `cursor_pos: v2f` (read_only)
- `scroll_delta: v2f` (read_only)
- `is_any_button_pressed: boolean` (read_only)
- `is_any_button_just_pressed: boolean` (read_only)
- `is_any_button_just_released: boolean` (read_only)
- `pressed_buttons: [string]` (read_only)
- `just_pressed_buttons: [string]` (read_only)
- `just_released_buttons: [string]` (read_only)

## EXAMPLE

```lua
the_debug:trace("cursor position: " .. tostring(the_mouse.cursor_pos))

if the_mouse:is_button_just_pressed("left") then
    the_debug:trace("mouse button was pressed")
end
```
