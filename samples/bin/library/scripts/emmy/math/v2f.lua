---@class v2f
local v2f = {
    ---@type number
    x = 0,

    ---@type number
    y = 0
}

---@overload fun(): v2f
---@overload fun(v: number): v2f
---@overload fun(x: number, y: number): v2f
---@overload fun(v: v2f): v2f
---@overload fun(v: v3f): v2f
---@overload fun(v: v4f): v2f
---@return v2f
function v2f.new(...) end

---@return v2f
function v2f.zero() end

---@return v2f
function v2f.unit() end

---@return v2f
function v2f.unit_x() end

---@return v2f
function v2f.unit_y() end

---@param l v2f
---@param r v2f
---@return boolean
function v2f.approximately(l, r) end

---@param l v2f
---@param r v2f
---@return number
function v2f.dot(l, r) end

---@param l v2f
---@param r v2f
---@return number
function v2f.abs_dot(l, r) end

---@param v v2f
---@return number
function v2f.length_squared(v) end

---@param v v2f
---@return number
function v2f.length(v) end

---@param l v2f
---@param r v2f
---@return number
function v2f.distance_squared(l, r) end

---@param l v2f
---@param r v2f
---@return number
function v2f.distance(l, r) end

---@param v v2f
---@return v2f
function v2f.normalized(v) end

---@param v v2f
---@return number
function v2f.minimum(v) end

---@param v v2f
---@return number
function v2f.maximum(v) end

---@param v v2f
---@param vmin v2f
---@return v2f
function v2f.minimized(v,vmin) end

---@param v v2f
---@param vmax v2f
---@return v2f
function v2f.maximized(v,vmax) end

---@param v v2f
---@param vmin v2f
---@param vmax v2f
---@return v2f
function v2f.clamped(v,vmin,vmax) end

---@param v v2f
---@return v2f
function v2f.saturated(v) end

---@param l v2f
---@param r v2f
---@param v number | v2f
---@return v2f
function v2f.lerp(l,r,v) end

---@param l v2f
---@param r v2f
---@param v v2f
---@return v2f
function v2f.inverse_lerp(l,r,v) end

---@param v v2f
---@return boolean
function v2f.contains_nan(v) end

---@type v2f
_G.v2f = _G.v2f or v2f
