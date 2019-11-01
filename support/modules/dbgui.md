# Dbgui

## Properties

- `visible: boolean, RW`

## Examples

```lua
local d = e2d.dbgui
local k = e2d.input.keyboard

local toggle_dbgui_key = e2d.keyboard_key.f12
if k:is_key_just_pressed(toggle_dbgui_key) then
    d.visible = not d.visible
end
```

