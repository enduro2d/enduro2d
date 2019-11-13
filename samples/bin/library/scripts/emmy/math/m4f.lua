---@class m4f
local m4f = {
    ---@type v4f[]
    rows = {}
}

---@overload fun(): m4f
---@overload fun(r0: v4f, r1: v4f, r2: v4f, r3: v4f): m4f
---@return m4f
function m4f.new(...) end

---@return m4f
function m4f.zero() end

---@return m4f
function m4f.identity() end

---@overload fun(x: number, y: number, z: number): m4f
---@overload fun(xyz: v4f): m4f
---@overload fun(xyz: v3f): m4f
---@overload fun(xy: v2f, z: number): m4f
---@return m4f
function m4f.make_scale(...) end

---@overload fun(x: number, y: number, z: number): m4f
---@overload fun(xyz: v4f): m4f
---@overload fun(xyz: v3f): m4f
---@overload fun(xy: v2f, z: number): m4f
---@return m4f
function m4f.make_translation(...) end

---@overload fun(a: degf | radf, x: number, y: number, z: number): m4f
---@overload fun(a: degf | radf, xyz: v4f): m4f
---@overload fun(a: degf | radf, xyz: v3f): m4f
---@overload fun(a: degf | radf, xy: v2f, z: number): m4f
---@overload fun(q: q4f): m4f
---@return m4f
function m4f.make_rotation(...) end

---@param t t2f | t3f
---@return m4f
function m4f.make_trs(t) end

---@param eye v3f
---@param at v3f
---@param up v3f
---@return m4f
function m4f.make_look_at_lh(eye, at, up) end

---@param eye v3f
---@param at v3f
---@param up v3f
---@return m4f
function m4f.make_look_at_rh(eye, at, up) end

---@overload fun(width: number, height: number, znear: number, zfar: number): m4f
---@overload fun(size: v2f, znear: number, zfar: number): m4f
---@return m4f
function m4f.make_orthogonal_lh(...) end

---@overload fun(width: number, height: number, znear: number, zfar: number): m4f
---@overload fun(size: v2f, znear: number, zfar: number): m4f
---@return m4f
function m4f.make_orthogonal_rh(...) end

---@param angle degf | radf
---@param aspect number
---@param znear number
---@param zfar number
---@return m4f
function m4f.make_perspective_lh(angle, aspect, znear, zfar) end

---@param angle degf | radf
---@param aspect number
---@param znear number
---@param zfar number
---@return m4f
function m4f.make_perspective_rh(angle, aspect, znear, zfar) end

---@param l m4f
---@param r m4f
---@return boolean
function m4f.approximately(l, r) end

---@param m m4f
---@return m4f, boolean
function m4f.inversed(m) end

---@param m m4f
---@return m4f
function m4f.transposed(m) end

---@param m m4f
---@return boolean
function m4f.contains_nan(m) end

---@type m4f
_G.m4f = _G.m4f or m4f
