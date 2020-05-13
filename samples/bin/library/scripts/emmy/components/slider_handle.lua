---@class slider_handle
local slider_handle = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: slider_handle)
---@param self slider_handle
function slider_handle.enable(self) end

---@overload fun(self: slider_handle)
---@param self slider_handle
function slider_handle.disable(self) end

---@type slider_handle
_G.slider_handle = _G.slider_handle or slider_handle
