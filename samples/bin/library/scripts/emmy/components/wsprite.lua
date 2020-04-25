---@class wsprite
local wsprite = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: wsprite)
---@param self wsprite
function wsprite.enable(self) end

---@overload fun(self: wsprite)
---@param self wsprite
function wsprite.disable(self) end

---@type wsprite
_G.wsprite = _G.wsprite or wsprite
