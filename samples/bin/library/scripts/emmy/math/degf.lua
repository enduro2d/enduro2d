---@class degf
local degf = {
    ---@type number
    value = 0
}

---@overload fun(): degf
---@overload fun(v: number): degf
---@overload fun(u: degf): degf
---@return degf
function degf.new() end

---@return degf
function degf.zero() end

---@param l degf
---@param r degf
---@return boolean
function degf.approximately(l, r) end

---@param v degf
---@param vmin degf
---@return degf
function degf.minimized(v,vmin) end

---@param v degf
---@param vmax degf
---@return degf
function degf.maximized(v,vmax) end

---@param v degf
---@param vmin degf
---@param vmax degf
---@return degf
function degf.clamped(v,vmin,vmax) end

---@param v degf
---@return degf
function degf.saturated(v) end

---@param v degf
---@return boolean
function degf.contains_nan(v) end

---@type degf
_G.degf = _G.degf or degf
