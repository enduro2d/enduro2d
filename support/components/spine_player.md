# Spine Player

## Properties

- `spine: spine_asset, RW`

## Functions

- `skin(name: string) -> void`
- `attachment(slot: string, name: string) -> void`
- `has_skin(name: string) -> boolean`
- `has_animation(name: string) -> boolean`
- `add_command(command: commands) -> void`

## Commands

- `clear_track_cmd(track: number)`
  - `track: number, RO`

- `set_anim_cmd(track: number, name: string)`
  - `track: number, RO`
  - `name: string, RO`
  - `loop: boolean, RW`
  - `end_message: string, RW`
  - `complete_messsage: string, RW`

- `add_anim_cmd(track: number, name: string)`
  - `track: number, RO`
  - `name: string, RO`
  - `loop: boolean, RW`
  - `delay: secf, RW`
  - `end_message: string, RW`
  - `complete_messsage: string, RW`

- `set_empty_anim_cmd(track: number)`
  - `track: number, RO`
  - `mix_duration: secf, RW`
  - `end_message: string, RW`
  - `complete_messsage: string, RW`

- `add_empty_anim_cmd(track: number)`
  - `track: number, RO`
  - `delay: secf, RW`
  - `mix_duration: secf, RW`
  - `end_message: string, RW`
  - `complete_messsage: string, RW`

## Events

- `custom_evt(name: str_hash)`
  - `name: str_hash, RO`
  - `int_value: number, RW`
  - `float_value: number, RW`
  - `string_value: string, RW`

- `end_evt(message: string)`
  - `message: string, RO`

- `complete_evt(message: string)`
  - `message: string, RO`

## Example

```lua
local M = {}

local keyboard = e2d.input.keyboard
local roar_key = e2d.keyboard_key.r

function M:on_update(go)
  local roar = keyboard:is_key_just_pressed(roar_key)
  if roar then
    local cmd = e2d.spine_player.set_anim_cmd.new(0, "roar")
    cmd.complete_message = "to_walk"
    go.spine_player:add_command(cmd)
  end
end

function M:on_event(go, type, event)
  local to_walk =
    type == "spine_player.complete_evt" and
    event.message == "to_walk"
  if to_walk then
    local cmd = e2d.spine_player.add_anim_cmd.new(0, "walk")
    cmd.loop = true
    go.spine_player:add_command(cmd)
  end
end

return M
```

