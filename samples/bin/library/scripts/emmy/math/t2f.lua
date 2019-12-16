---@class t2f
local t2f = {
    ---@type v2f
    translation = v2f.zero(),

    ---@type number
    rotation = 0,

    ---@type v2f
    scale = v2f.unit()
}

---@overload fun(): t2f
---@return t2f
function t2f.new() end

---@return t2f
function t2f.zero() end

---@return t2f
function t2f.identity() end

---@param t v2f
---@return t2f
function t2f.make_translation(t) end

---@param r number
---@return t2f
function t2f.make_rotation(r) end

---@param s v2f
---@return t2f
function t2f.make_scale(s) end

---@param l t2f
---@param r t2f
---@return boolean
function t2f.approximately(l, r) end

---@type t2f
_G.t2f = _G.t2f or t2f
