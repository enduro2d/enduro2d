---@class scroll
local scroll = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: scroll)
---@param self scroll
function scroll.enable(self) end

---@overload fun(self: scroll)
---@param self scroll
function scroll.disable(self) end

---@type scroll
_G.scroll = _G.scroll or scroll
