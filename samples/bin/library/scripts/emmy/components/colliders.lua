-- -----------------------------------------------------------------------------
--
-- rect_collider
--
-- -----------------------------------------------------------------------------

---@class rect_collider
local rect_collider = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type v2f
    size = v2f.unit(),

    ---@type v2f
    offset = v2f.zero()
}

---@overload fun(self: rect_collider)
---@param self rect_collider
function rect_collider.enable(self) end

---@overload fun(self: rect_collider)
---@param self rect_collider
function rect_collider.disable(self) end

---@type rect_collider
_G.rect_collider = _G.rect_collider or rect_collider

-- -----------------------------------------------------------------------------
--
-- circle_collider
--
-- -----------------------------------------------------------------------------

---@class circle_collider
local circle_collider = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type number
    radius = 1.0,

    ---@type v2f
    offset = v2f.zero()
}

---@overload fun(self: circle_collider)
---@param self circle_collider
function circle_collider.enable(self) end

---@overload fun(self: circle_collider)
---@param self circle_collider
function circle_collider.disable(self) end

---@type circle_collider
_G.circle_collider = _G.circle_collider or circle_collider

-- -----------------------------------------------------------------------------
--
-- polygon_collider
--
-- -----------------------------------------------------------------------------

---@class polygon_collider
local polygon_collider = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type v2f[]
    points = {
        v2f.new(-0.5, -0.5),
        v2f.new( 0.5, -0.5),
        v2f.new( 0.5,  0.5),
        v2f.new(-0.5,  0.5)},

    ---@type v2f
    offset = v2f.zero()
}

---@overload fun(self: polygon_collider)
---@param self polygon_collider
function polygon_collider.enable(self) end

---@overload fun(self: polygon_collider)
---@param self polygon_collider
function polygon_collider.disable(self) end

---@type polygon_collider
_G.polygon_collider = _G.polygon_collider or polygon_collider
