---@class behaviour
local behaviour = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type table
    meta = {}
}

---@overload fun(self: behaviour)
---@param self behaviour
function behaviour.enable(self) end

---@overload fun(self: behaviour)
---@param self behaviour
function behaviour.disable(self) end

---@type behaviour
_G.behaviour = _G.behaviour or behaviour
