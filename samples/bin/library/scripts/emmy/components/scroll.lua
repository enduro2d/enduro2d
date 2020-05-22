---@class scroll
local scroll = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    vertical = true,

    ---@type boolean
    horizontal = true,

    ---@type number
    inertia = 0.135,

    ---@type number
    elasticity = 0.1,

    ---@type number
    sensitivity = 1.0
}

---@overload fun(self: scroll)
---@param self scroll
function scroll.enable(self) end

---@overload fun(self: scroll)
---@param self scroll
function scroll.disable(self) end

---@type scroll
_G.scroll = _G.scroll or scroll
