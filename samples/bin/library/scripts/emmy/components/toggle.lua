---@class toggle
local toggle = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    pressed = false,

    ---@type toggle_style_asset
    style = nil
}

---@overload fun(self: toggle)
---@param self toggle
function toggle.enable(self) end

---@overload fun(self: toggle)
---@param self toggle
function toggle.disable(self) end

---@type toggle
_G.toggle = _G.toggle or toggle
