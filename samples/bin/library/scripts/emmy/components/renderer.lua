---@class renderer
local renderer = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type t3f
    transform = t3f.identity(),

    ---@type v3f
    translation = v3f.zero(),

    ---@type v3f
    rotation = v3f.zero(),

    ---@type v3f
    scale = v3f.unit()
}

---@overload fun(self: renderer)
---@param self renderer
function renderer.enable(self) end

---@overload fun(self: renderer)
---@param self renderer
function renderer.disable(self) end

---@type renderer
_G.renderer = _G.renderer or renderer
