---@class renderer
local renderer = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: renderer)
---@param self renderer
function renderer.enable(self) end

---@overload fun(self: renderer)
---@param self renderer
function renderer.disable(self) end

---@type renderer
_G.renderer = _G.renderer or renderer
