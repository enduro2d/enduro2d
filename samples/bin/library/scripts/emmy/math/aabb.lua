---@class aabb
local aabb = {
    ---@type v3f
    position = v3f.zero(),

    ---@type v3f
    size = v3f.zero()
}

---@overload fun(): aabb
---@overload fun(r: aabb): aabb
---@overload fun(w: number, h: number, l: number): aabb
---@overload fun(x: number, y: number, z: number, w: number, h: number, l: number): aabb
---@overload fun(s: v3f): aabb
---@overload fun(p: v3f, s: v3f): aabb
---@return aabb
function aabb.new(...) end

---@return aabb
function aabb.zero() end

---@return aabb
function aabb.unit() end

---@overload fun(r: aabb): aabb
---@overload fun(x1: number, y1: number, z1: number, x2: number, y2: number, z2: number): aabb
---@overload fun(p1: v3f, p2: v3f): aabb
---@param r aabb
---@return aabb
function aabb.make_minmax(r) end

---@param l aabb
---@param r aabb
---@return boolean
function aabb.approximately(l, r) end

---@param r aabb
---@return v3f
function aabb.minimum(r) end

---@param r aabb
---@return v3f
function aabb.maximum(r) end

---@param r aabb
---@return number
function aabb.volume(r) end

---@param r aabb
---@return number
function aabb.abs_volume(r) end

---@param l aabb
---@param r aabb
---@return aabb
function aabb.merged(l, r) end

---@param r aabb
---@param p v3f
---@return boolean
function aabb.inside(r, p) end

---@param l aabb
---@param r aabb
---@return boolean
function aabb.overlaps(l, r) end

---@param r aabb
---@param p v3f
---@return v3f
function aabb.normalized_to_point(r, p) end

---@param r aabb
---@param p v3f
---@return v3f
function aabb.point_to_normalized(r, p) end

---@type aabb
_G.aabb = _G.aabb or aabb
