-- -----------------------------------------------------------------------------
--
-- rect_mask
--
-- -----------------------------------------------------------------------------

---@class rect_mask
local rect_mask = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type v2f
    size = v2f.unit(),

    ---@type v2f
    offset = v2f.zero(),

    ---@type boolean
    visible = false
}

---@overload fun(self: rect_mask)
---@param self rect_mask
function rect_mask.enable(self) end

---@overload fun(self: rect_mask)
---@param self rect_mask
function rect_mask.disable(self) end

---@type rect_mask
_G.rect_mask = _G.rect_mask or rect_mask

-- -----------------------------------------------------------------------------
--
-- circle_mask
--
-- -----------------------------------------------------------------------------

---@class circle_mask
local circle_mask = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type number
    radius = 1.0,

    ---@type v2f
    offset = v2f.zero(),

    ---@type boolean
    visible = false
}

---@overload fun(self: circle_mask)
---@param self circle_mask
function circle_mask.enable(self) end

---@overload fun(self: circle_mask)
---@param self circle_mask
function circle_mask.disable(self) end

---@type circle_mask
_G.circle_mask = _G.circle_mask or circle_mask

-- -----------------------------------------------------------------------------
--
-- sprite_mask
--
-- -----------------------------------------------------------------------------

---@class sprite_mask
local sprite_mask = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type sprite_asset
    sprite = nil,

    ---@type number
    threshold = 0.1,

    ---@type v2f
    offset = v2f.zero(),

    ---@type boolean
    visible = false
}

---@overload fun(self: sprite_mask)
---@param self sprite_mask
function sprite_mask.enable(self) end

---@overload fun(self: sprite_mask)
---@param self sprite_mask
function sprite_mask.disable(self) end

---@type sprite_mask
_G.sprite_mask = _G.sprite_mask or sprite_mask
