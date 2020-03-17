---@class slider
local slider = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type number
    min_value = 0.0,

    ---@type number
    max_value = 1.0,

    ---@type number
    value = 0.0,

    ---@type boolean
    whole_numbers = false,

    ---@type slider_directions
    direction = slider.directions.row
}

---@class slider_directions
slider.directions = {
    row = "row",
    row_reversed = "row_reversed",
    column = "column",
    column_reversed = "column_reversed"
}

---@overload fun(self: slider)
---@param self slider
function slider.enable(self) end

---@overload fun(self: slider)
---@param self slider
function slider.disable(self) end

---@type slider
_G.slider = _G.slider or slider