---@class handle
local handle = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: handle)
---@param self handle
function handle.enable(self) end

---@overload fun(self: handle)
---@param self handle
function handle.disable(self) end

---@type handle
_G.handle = _G.handle or handle
