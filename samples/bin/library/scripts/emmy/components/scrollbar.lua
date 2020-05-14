---@class scrollbar
local scrollbar = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type number
    value = 0.0,

    ---@type scrollbar_directions
    direction = scrollbar.directions.row
}

---@class scrollbar_directions
scrollbar.directions = {
    row = "row",
    row_reversed = "row_reversed",
    column = "column",
    column_reversed = "column_reversed"
}

---@overload fun(self: scrollbar)
---@param self scrollbar
function scrollbar.enable(self) end

---@overload fun(self: scrollbar)
---@param self scrollbar
function scrollbar.disable(self) end

---@type scrollbar
_G.scrollbar = _G.scrollbar or scrollbar
