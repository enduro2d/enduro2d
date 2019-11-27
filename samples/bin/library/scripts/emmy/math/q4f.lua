---@class q4f
local q4f = {
    ---@type number
    x = 0,

    ---@type number
    y = 0,

    ---@type number
    z = 0,

    ---@type number
    w = 0
}

---@overload fun(): q4f
---@overload fun(q: q4f): q4f
---@overload fun(x: number, y: number, z: number, w: number): q4f
---@overload fun(v: v4f): q4f
---@return q4f
function q4f.new(...) end

---@return q4f
function q4f.zero() end

---@return q4f
function q4f.identity() end

---@param angle number
---@param axis v3f
---@return q4f
function q4f.make_from_axis_angle(angle, axis) end

---@param roll number
---@param pitch number
---@param yaw number
---@return q4f
function q4f.make_from_euler_angles(roll, pitch, yaw) end

---@param l q4f
---@param r q4f
---@return boolean
function q4f.approximately(l, r) end

---@param l q4f
---@param r q4f
---@return number
function q4f.dot(l, r) end

---@param l q4f
---@param r q4f
---@return number
function q4f.abs_dot(l, r) end

---@param q q4f
---@return number
function q4f.length_squared(q) end

---@param q q4f
---@return number
function q4f.length(q) end

---@param q q4f
---@return q4f
function q4f.normalized(q) end

---@param l q4f
---@param r q4f
---@param v number
---@return q4f
function q4f.lerp(l, r, v) end

---@param q q4f
---@return q4f
function q4f.inversed(q) end

---@param q q4f
---@return q4f
function q4f.conjugated(q) end

---@param q q4f
---@return q4f
function q4f.contains_nan(q) end

---@type q4f
_G.q4f = _G.q4f or q4f
