---@class m3f
local m3f = {
    ---@type v3f[]
    rows = {}
}

---@overload fun(): m3f
---@overload fun(r0: v3f, r1: v3f, r2: v3f): m3f
---@return m3f
function m3f.new() end

---@return m3f
function m3f.zero() end

---@return m3f
function m3f.identity() end

---@overload fun(x: number, y: number, z: number): m3f
---@overload fun(xyz: v4f): m3f
---@overload fun(xyz: v3f): m3f
---@overload fun(xy: v2f, z: number): m3f
---@param x number
---@param y number
---@param z number
---@return m3f
function m3f.make_scale(x,y,z) end

---@overload fun(a: degf | radf, x: number, y: number, z: number): m3f
---@overload fun(a: degf | radf, xyz: v4f): m3f
---@overload fun(a: degf | radf, xyz: v3f): m3f
---@overload fun(a: degf | radf, xy: v2f, z: number): m3f
---@overload fun(q: q4f): m3f
---@param a degf | radf
---@param x number
---@param y number
---@param z number
---@return m3f
function m3f.make_rotation(a,x,y,z) end

---@param l m3f
---@param r m3f
---@return boolean
function m3f.approximately(l, r) end

---@param m m3f
---@return m3f, boolean
function m3f.inversed(m) end

---@param m m3f
---@return m3f
function m3f.transposed(m) end

---@param m m3f
---@return boolean
function m3f.contains_nan(m) end

---@type m3f
_G.m3f = _G.m3f or m3f