---@class radf
local radf = {
    ---@type number
    value = 0
}

---@overload fun(): radf
---@overload fun(v: number): radf
---@overload fun(u: radf): radf
---@return radf
function radf.new() end

---@return radf
function radf.zero() end

---@param l radf
---@param r radf
---@return boolean
function radf.approximately(l, r) end

---@param v radf
---@param vmin radf
---@return radf
function radf.minimized(v,vmin) end

---@param v radf
---@param vmax radf
---@return radf
function radf.maximized(v,vmax) end

---@param v radf
---@param vmin radf
---@param vmax radf
---@return radf
function radf.clamped(v,vmin,vmax) end

---@param v radf
---@return radf
function radf.saturated(v) end

---@param v radf
---@return boolean
function radf.contains_nan(v) end

---@type radf
_G.radf = _G.radf or radf
