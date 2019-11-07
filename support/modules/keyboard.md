# KEYBOARD

## FUNCTIONS

- `is_key_pressed(key: string): boolean`
- `is_key_just_pressed(key: string): boolean`
- `is_key_just_released(key: string): boolean`

## PROPERTIES

- `input_text: string` (read_only)
- `is_any_key_pressed: boolean` (read_only)
- `is_any_key_just_pressed: boolean` (read_only)
- `is_any_key_just_released: boolean` (read_only)
- `pressed_keys: [string]` (read_only)
- `just_pressed_keys: [string]` (read_only)
- `just_released_keys: [string]` (read_only)

## EXAMPLE

```lua
if the_keyboard:is_key_pressed("w") then
    the_debug:trace("move forward")
end
```
