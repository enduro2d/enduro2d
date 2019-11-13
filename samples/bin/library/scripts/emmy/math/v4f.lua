---@class v4f
local v4f = {
    ---@type number
    x = 0,

    ---@type number
    y = 0,

    ---@type number
    z = 0,

    ---@type number
    w = 0
}

---@overload fun(): v4f
---@overload fun(v: number): v4f
---@overload fun(x: number, y: number, z: number, w: number): v4f
---@overload fun(v: v2f, z: number, w: number): v4f
---@overload fun(v: v3f, w: number): v4f
---@overload fun(v: v4f): v4f
---@return v4f
function v4f.new(...) end

---@return v4f
function v4f.zero() end

---@return v4f
function v4f.unit() end

---@return v4f
function v4f.unit_x() end

---@return v4f
function v4f.unit_y() end

---@return v4f
function v4f.unit_z() end

---@return v4f
function v4f.unit_w() end

---@param l v4f
---@param r v4f
---@return boolean
function v4f.approximately(l, r) end

---@param l v4f
---@param r v4f
---@return number
function v4f.dot(l, r) end

---@param l v4f
---@param r v4f
---@return number
function v4f.abs_dot(l, r) end

---@param v v4f
---@return number
function v4f.length_squared(v) end

---@param v v4f
---@return number
function v4f.length(v) end

---@param l v4f
---@param r v4f
---@return number
function v4f.distance_squared(l, r) end

---@param l v4f
---@param r v4f
---@return number
function v4f.distance(l, r) end

---@param v v4f
---@return v4f
function v4f.normalized(v) end

---@param v v4f
---@return number
function v4f.minimum(v) end

---@param v v4f
---@return number
function v4f.maximum(v) end

---@param v v4f
---@param vmin v4f
---@return v4f
function v4f.minimized(v,vmin) end

---@param v v4f
---@param vmax v4f
---@return v4f
function v4f.maximized(v,vmax) end

---@param v v4f
---@param vmin v4f
---@param vmax v4f
---@return v4f
function v4f.clamped(v,vmin,vmax) end

---@param v v4f
---@return v4f
function v4f.saturated(v) end

---@param l v4f
---@param r v4f
---@param v number | v4f
---@return v4f
function v4f.lerp(l,r,v) end

---@param l v4f
---@param r v4f
---@param v v4f
---@return v4f
function v4f.inverse_lerp(l,r,v) end

---@param v v4f
---@return boolean
function v4f.contains_nan(v) end

---@type v4f
_G.v4f = _G.v4f or v4f
