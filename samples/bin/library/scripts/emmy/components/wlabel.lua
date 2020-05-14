---@class wlabel
local wlabel = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: wlabel)
---@param self wlabel
function wlabel.enable(self) end

---@overload fun(self: wlabel)
---@param self wlabel
function wlabel.disable(self) end

---@type wlabel
_G.wlabel = _G.wlabel or wlabel
