---@class t3f
local t3f = {
    ---@type v3f
    translation = v3f.zero(),

    ---@type q4f
    rotation = q4f.identity(),

    ---@type v3f
    scale = v3f.unit()
}

---@overload fun(): t3f
---@overload fun(t: t3f): t3f
---@overload fun(t: v3f, r: q4f, s: v3f): t3f
---@return t3f
function t3f.new() end

---@return t3f
function t3f.zero() end

---@return t3f
function t3f.identity() end

---@param t v3f
---@return t3f
function t3f.make_translation(t) end

---@param r q4f
---@return t3f
function t3f.make_rotation(r) end

---@param s v3f
---@return t3f
function t3f.make_scale(s) end

---@param l t3f
---@param r t3f
---@return boolean
function t3f.approximately(l, r) end

---@param t t3f
---@return boolean
function t3f.contains_nan(t) end

---@type t3f
_G.t3f = _G.t3f or t3f
