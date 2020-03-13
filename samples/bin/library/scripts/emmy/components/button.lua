---@class button
local button = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    pressed = false,

    ---@type button_style_asset
    style = nil
}

---@overload fun(self: button)
---@param self button
function button.enable(self) end

---@overload fun(self: button)
---@param self button
function button.disable(self) end

---@type button
_G.button = _G.button or button
