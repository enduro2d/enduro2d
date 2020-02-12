---@class layout
local layout = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    dirty = false,

    ---@type boolean
    was_moved = false,

    ---@type layout_modes
    mode = layout.modes.horizontal,

    ---@type layout_haligns
    halign = layout.haligns.center,

    ---@type layout_valigns
    valign = layout.valigns.center,

    ---@type v2f
    size = v2f.zero(),

    ---@type v2f
    margin = v2f.zero(),

    ---@type v2f
    padding = v2f.zero()
}

---@class layout_modes
layout.modes = {
    horizontal = "horizontal",
    vertical = "vertical"
}

---@class layout_haligns
layout.haligns = {
    left = "left",
    center = "center",
    right = "right",
    space_around = "space_around",
    space_evenly = "space_evenly",
    space_between = "space_between"
}

---@class layout_valigns
layout.valigns = {
    top = "top",
    center = "center",
    bottom = "bottom",
    space_around = "space_around",
    space_evenly = "space_evenly",
    space_between = "space_between"
}

---@overload fun(self: layout)
---@param self layout
function layout.enable(self) end

---@overload fun(self: layout)
---@param self layout
function layout.disable(self) end

---@type layout
_G.layout = _G.layout or layout
