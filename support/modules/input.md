# INPUT

## PROPERTIES

- `mouse: mouse` (read_only)
- `keyboard: keyboard` (read_only)

## EXAMPLE

```lua
if the_input.mouse.is_any_button_just_pressed then
    the_debug:trace("some mouse button was pressed")
end

if the_input.keyboard:is_key_just_pressed("space") then
    the_debug:trace("`space` key was pressed")
end
```
