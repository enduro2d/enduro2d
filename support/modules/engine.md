# Engine

## Properties

- `time: number, RO`
- `delta_time: number, RO`
- `frame_rate: number, RO`
- `frame_count: number, RO`
- `realtime_time: number, RO`

## Examples

```lua
local d = e2d.debug
local e = e2d.engine
d:trace("FPS: " .. e.frame_rate)
```

