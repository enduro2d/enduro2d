---@class progress
local progress = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type number
    value = 0.0,

    ---@type progress_directions
    direction = progress.directions.row
}

---@class progress_directions
progress.directions = {
    row = "row",
    row_reversed = "row_reversed",
    column = "column",
    column_reversed = "column_reversed"
}

---@overload fun(self: progress)
---@param self progress
function progress.enable(self) end

---@overload fun(self: progress)
---@param self progress
function progress.disable(self) end

---@type progress
_G.progress = _G.progress or progress
