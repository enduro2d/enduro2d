---@class m3f
local m3f = {
    ---@type v3f[]
    rows = {}
}

---@overload fun(): m3f
---@overload fun(r0: v3f, r1: v3f, r2: v3f): m3f
---@return m3f
function m3f.new(...) end

---@return m3f
function m3f.zero() end

---@return m3f
function m3f.identity() end

---@overload fun(x: number, y: number, z: number): m3f
---@overload fun(xyz: v4f): m3f
---@overload fun(xyz: v3f): m3f
---@overload fun(xy: v2f, z: number): m3f
---@return m3f
function m3f.make_scale(...) end

---@overload fun(a: number, x: number, y: number, z: number): m3f
---@overload fun(a: number, xyz: v3f): m3f
---@overload fun(r: number, p: number, y: number): m3f
---@overload fun(rpy: v3f): m3f
---@return m3f
function m3f.make_rotation(...) end

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

---@type m3f
_G.m3f = _G.m3f or m3f
