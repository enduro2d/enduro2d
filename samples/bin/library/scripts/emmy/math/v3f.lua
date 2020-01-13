---@class v3f
local v3f = {
    ---@type number
    x = 0,

    ---@type number
    y = 0,

    ---@type number
    z = 0
}

---@overload fun(): v3f
---@overload fun(v: number): v3f
---@overload fun(x: number, y: number, z: number): v3f
---@overload fun(v: v2f, z: number): v3f
---@overload fun(v: v3f): v3f
---@overload fun(v: v4f): v3f
---@return v3f
function v3f.new(...) end

---@return v3f
function v3f.zero() end

---@return v3f
function v3f.unit() end

---@return v3f
function v3f.unit_x() end

---@return v3f
function v3f.unit_y() end

---@return v3f
function v3f.unit_z() end

---@param l v3f
---@param r v3f
---@return boolean
function v3f.approximately(l, r) end

---@param l v3f
---@param r v3f
---@return number
function v3f.dot(l, r) end

---@param l v3f
---@param r v3f
---@return number
function v3f.abs_dot(l, r) end

---@param l v3f
---@param r v3f
---@return v3f
function v3f.cross(l, r) end

---@param v v3f
---@return number
function v3f.length_squared(v) end

---@param v v3f
---@return number
function v3f.length(v) end

---@param l v3f
---@param r v3f
---@return number
function v3f.distance_squared(l, r) end

---@param l v3f
---@param r v3f
---@return number
function v3f.distance(l, r) end

---@param v v3f
---@return v3f
function v3f.normalized(v) end

---@param v v3f
---@return number
function v3f.minimum(v) end

---@param v v3f
---@return number
function v3f.maximum(v) end

---@param v v3f
---@param vmin v3f
---@return v3f
function v3f.minimized(v,vmin) end

---@param v v3f
---@param vmax v3f
---@return v3f
function v3f.maximized(v,vmax) end

---@param v v3f
---@param vmin v3f
---@param vmax v3f
---@return v3f
function v3f.clamped(v,vmin,vmax) end

---@param v v3f
---@return v3f
function v3f.saturated(v) end

---@param l v3f
---@param r v3f
---@param v number | v3f
---@return v3f
function v3f.lerp(l,r,v) end

---@param l v3f
---@param r v3f
---@param v v3f
---@return v3f
function v3f.inverse_lerp(l,r,v) end

---@type v3f
_G.v3f = _G.v3f or v3f
