---@class sprite_renderer
local sprite_renderer = {
    ---@type color32
    tint = color32.white(),

    ---@type v2f
    scale = v2f.unit(),

    ---@type sprite_renderer_modes
    mode = sprite_renderer.modes.simple,

    ---@type sprite_renderer_blendings
    blending = sprite_renderer.blendings.normal,

    ---@type boolean
    filtering = true,

    ---@type sprite_asset
    sprite = nil
}

---@class sprite_renderer_modes
sprite_renderer.modes = {
    simple = "simple",
    sliced = "sliced"
}

---@class sprite_renderer_blendings
sprite_renderer.blendings = {
    normal = "normal",
    additive = "additive",
    multiply = "multiply",
    screen = "screen"
}

---@type sprite_renderer
_G.sprite_renderer = _G.sprite_renderer or sprite_renderer
