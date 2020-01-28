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
---@overload fun(xyz: v3f): m4f
---@overload fun(xy: v2f, z: number): m4f
---@return m4f
function m4f.make_scale(...) end

---@overload fun(x: number, y: number, z: number): m4f
---@overload fun(xyz: v3f): m4f
---@overload fun(xy: v2f, z: number): m4f
---@return m4f
function m4f.make_translation(...) end

---@overload fun(a: number, x: number, y: number, z: number): m4f
---@overload fun(a: number, xyz: v3f): m4f
---@overload fun(r: number, p: number, y: number): m3f
---@overload fun(rpy: v3f): m3f
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
function m4f.make_orthographic_lh_zo(...) end

---@overload fun(width: number, height: number, znear: number, zfar: number): m4f
---@overload fun(size: v2f, znear: number, zfar: number): m4f
---@return m4f
function m4f.make_orthographic_lh_no(...) end

---@overload fun(width: number, height: number, znear: number, zfar: number): m4f
---@overload fun(size: v2f, znear: number, zfar: number): m4f
---@return m4f
function m4f.make_orthographic_rh_zo(...) end

---@overload fun(width: number, height: number, znear: number, zfar: number): m4f
---@overload fun(size: v2f, znear: number, zfar: number): m4f
---@return m4f
function m4f.make_orthographic_rh_no(...) end

---@param angle number
---@param aspect number
---@param znear number
---@param zfar number
---@return m4f
function m4f.make_perspective_lh_zo(angle, aspect, znear, zfar) end

---@param angle number
---@param aspect number
---@param znear number
---@param zfar number
---@return m4f
function m4f.make_perspective_lh_no(angle, aspect, znear, zfar) end

---@param angle number
---@param aspect number
---@param znear number
---@param zfar number
---@return m4f
function m4f.make_perspective_rh_zo(angle, aspect, znear, zfar) end

---@param angle number
---@param aspect number
---@param znear number
---@param zfar number
---@return m4f
function m4f.make_perspective_rh_no(angle, aspect, znear, zfar) end

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

---@type m4f
_G.m4f = _G.m4f or m4f
