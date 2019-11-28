---@class rect_collider
local rect_collider = {
    ---@type v2f
    size = v2f.zero(),

    ---@type v2f
    pivot = v2f.new(0.5)
}

---@class circle_collider
local circle_collider = {
    ---@type number
    radius = 0,

    ---@type v2f
    pivot = v2f.new(0.5)
}

---@class polygon_collider
local polygon_collider = {
    ---@type v2f[]
    points = {},

    ---@type v2f
    pivot = v2f.new(0.5)
}

---@type rect_collider
_G.rect_collider = _G.rect_collider or rect_collider

---@type circle_collider
_G.circle_collider = _G.circle_collider or circle_collider

---@type polygon_collider
_G.polygon_collider = _G.polygon_collider or polygon_collider
