-- -----------------------------------------------------------------------------
--
-- layout
--
-- -----------------------------------------------------------------------------

---@class layout
local layout = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    dirty = false,

    ---@type layout_modes
    mode = layout.modes.horizontal,

    ---@type layout_haligns
    halign = layout.haligns.center,

    ---@type layout_valigns
    valign = layout.valigns.center
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
    space_between = "space_between"
}

---@class layout_valigns
layout.valigns = {
    top = "top",
    center = "center",
    bottom = "bottom",
    space_around = "space_around",
    space_between = "space_between"
}

---@overload fun(self: layout)
---@param self layout
function layout.enable(self) end

---@overload fun(self: layout)
---@param self layout
function layout.disable(self) end

-- -----------------------------------------------------------------------------
--
-- layout_item
--
-- -----------------------------------------------------------------------------

---@class layout_item
local layout_item = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    dirty = false,

    ---@type v2f
    size = v2f.unit(),

    ---@type v2f
    padding = v2f.zero()
}

---@overload fun(self: layout_item)
---@param self layout_item
function layout_item.enable(self) end

---@overload fun(self: layout_item)
---@param self layout_item
function layout_item.disable(self) end

-- -----------------------------------------------------------------------------
--
-- global
--
-- -----------------------------------------------------------------------------

---@type layout
_G.layout = _G.layout or layout

---@type layout_item
_G.layout_item = _G.layout_item or layout_item
