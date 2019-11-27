---@class rect
local rect = {
    ---@type v2f
    position = v2f.zero(),

    ---@type v2f
    size = v2f.zero()
}

---@overload fun(): rect
---@overload fun(r: rect): rect
---@overload fun(w: number, h: number): rect
---@overload fun(x: number, y: number, w: number, h: number): rect
---@overload fun(s: v2f): rect
---@overload fun(p: v2f, s: v2f): rect
---@return rect
function rect.new(...) end

---@return rect
function rect.zero() end

---@return rect
function rect.unit() end

---@overload fun(r: rect): rect
---@overload fun(x1: number, y1: number, x2: number, y2: number): rect
---@overload fun(p1: v2f, p2: v2f): rect
---@param r rect
---@return rect
function rect.make_minmax(r) end

---@param l rect
---@param r rect
---@return boolean
function rect.approximately(l, r) end

---@param r rect
---@return v2f
function rect.minimum(r) end

---@param r rect
---@return v2f
function rect.maximum(r) end

---@param r rect
---@return number
function rect.area(r) end

---@param r rect
---@return number
function rect.abs_area(r) end

---@param l rect
---@param r rect
---@return rect
function rect.merged(l, r) end

---@param r rect
---@param p v2f
---@return boolean
function rect.inside(r, p) end

---@param l rect
---@param r rect
---@return boolean
function rect.overlaps(l, r) end

---@param r rect
---@param p v2f
---@return v2f
function rect.normalized_to_point(r, p) end

---@param r rect
---@param p v2f
---@return v2f
function rect.point_to_normalized(r, p) end

---@param r rect
---@return boolean
function rect.contains_nan(r) end

---@type rect
_G.rect = _G.rect or rect
