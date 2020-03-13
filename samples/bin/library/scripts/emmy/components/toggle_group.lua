---@class toggle_group
local toggle_group = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    allow_switch_off = false,

    ---@type boolean
    allow_multiple_on = false
}

---@overload fun(self: toggle_group)
---@param self toggle_group
function toggle_group.enable(self) end

---@overload fun(self: toggle_group)
---@param self toggle_group
function toggle_group.disable(self) end

---@type toggle_group
_G.toggle_group = _G.toggle_group or toggle_group
