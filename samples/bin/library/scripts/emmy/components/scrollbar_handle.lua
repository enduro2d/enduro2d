---@class scrollbar_handle
local scrollbar_handle = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: scrollbar_handle)
---@param self scrollbar_handle
function scrollbar_handle.enable(self) end

---@overload fun(self: scrollbar_handle)
---@param self scrollbar_handle
function scrollbar_handle.disable(self) end

---@type scrollbar_handle
_G.scrollbar_handle = _G.scrollbar_handle or scrollbar_handle
