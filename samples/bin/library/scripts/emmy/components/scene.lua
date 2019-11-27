---@class scene
local scene = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type integer
    depth = 0
}

---@overload fun(self: scene)
---@param self scene
function scene.enable(self) end

---@overload fun(self: scene)
---@param self scene
function scene.disable(self) end

---@type scene
_G.scene = _G.scene or scene
