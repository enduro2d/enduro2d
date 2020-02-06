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

    ---@type layout_modes
    mode = layout.modes.vertical,
}

---@class layout_modes
layout.modes = {
    vertical = "vertical",
    horizontal = "horizontal"
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
    disabled = false
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
