---@class mask
local mask = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    visible = false
}

---@overload fun(self: mask)
---@param self mask
function mask.enable(self) end

---@overload fun(self: mask)
---@param self mask
function mask.disable(self) end

---@type mask
_G.mask = _G.mask or mask
