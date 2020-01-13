---@class m2f
local m2f = {
    ---@type v2f[]
    rows = {}
}

---@overload fun(): m2f
---@overload fun(r0: v2f, r1: v2f): m2f
---@return m2f
function m2f.new(...) end

---@return m2f
function m2f.zero() end

---@return m2f
function m2f.identity() end

---@overload fun(x: number, y: number): m2f
---@overload fun(xy: v2f): m2f
---@return m2f
function m2f.make_scale(...) end

---@param a number
---@return m2f
function m2f.make_rotation(a) end

---@param l m2f
---@param r m2f
---@return boolean
function m2f.approximately(l, r) end

---@param m m2f
---@return m2f, boolean
function m2f.inversed(m) end

---@param m m2f
---@return m2f
function m2f.transposed(m) end

---@type m2f
_G.m2f = _G.m2f or m2f
