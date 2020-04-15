---@class layout
local layout = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    dirty = false,

    ---@type layout_directions
    direction = layout.directions.ltr,

    ---@type layout_align_modes
    align_items = layout.align_modes.flex_start,

    ---@type layout_align_modes
    align_conten = layout.align_modes.flex_start,

    ---@type layout_justify_modes
    justify_content = layout.justify_modes.flex_start,

    ---@type layout_flex_wraps
    flex_wrap = layout.flex_wraps.no_wrap,

    ---@type layout_flex_directions
    flex_direction = layout.flex_directions.row
}

---@class layout_directions
layout.directions = {
    ltr = "ltr",
    rtl = "rtl"
}

---@class layout_align_modes
layout.align_modes = {
    flex_start = "flex_start",
    center = "center",
    flex_end = "flex_end",
    space_between = "space_between",
    space_around = "space_around"
}

---@class layout_justify_modes
layout.justify_modes = {
    flex_start = "flex_start",
    center = "center",
    flex_end = "flex_end",
    space_between = "space_between",
    space_around = "space_around",
    space_evenly = "space_evenly"
}

---@class layout_flex_wraps
layout.flex_wraps = {
    no_wrap = "no_wrap",
    wrap = "wrap",
    wrap_reversed = "wrap_reversed"
}

---@class layout_flex_directions
layout.flex_directions = {
    row = "row",
    row_reversed = "row_reversed",
    column = "column",
    column_reversed = "column_reversed"
}

---@overload fun(self: layout)
---@param self layout
function layout.enable(self) end

---@overload fun(self: layout)
---@param self layout
function layout.disable(self) end

---@type layout
_G.layout = _G.layout or layout
