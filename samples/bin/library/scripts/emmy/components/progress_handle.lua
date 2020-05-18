---@class progress_handle
local progress_handle = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: progress_handle)
---@param self progress_handle
function progress_handle.enable(self) end

---@overload fun(self: progress_handle)
---@param self progress_handle
function progress_handle.disable(self) end

---@type progress_handle
_G.progress_handle = _G.progress_handle or progress_handle
