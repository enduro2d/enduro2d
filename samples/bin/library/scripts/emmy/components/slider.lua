---@class slider
local slider = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type button_style_asset
    style = nil
}

---@overload fun(self: slider)
---@param self slider
function slider.enable(self) end

---@overload fun(self: slider)
---@param self slider
function slider.disable(self) end

---@type slider
_G.slider = _G.slider or slider
