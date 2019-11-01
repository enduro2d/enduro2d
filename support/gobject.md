# GObject

## Properties

- `alive: boolean, RO`
- `valid: boolean, RO`

## Functions

- `destroy() -> void`

## Components

- `actor: actor, RO`
- `behaviour: behaviour, RO`
- `camera: camera, RO`
- `flipbook_player: flipbook_player, RO`
- `label: label, RO`
- `model_renderer: model_renderer, RO`
- `renderer: renderer, RO`
- `scene: scene, RO`
- `spine_player: spine_player, RO`
- `sprite_renderer: sprite_renderer, RO`

## Example

```lua
local M = {}

function M:on_init(go)
  self.life = 2
end

function M:on_update(go)
  self.life = self.life - e2d.engine.delta_time
  if self.life <= 0 then
    go:destroy()
  end
end

return M
```
