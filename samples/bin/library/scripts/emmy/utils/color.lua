---@class color
local color = {
    ---@type number
    r = 1.0,

    ---@type number
    g = 1.0,

    ---@type number
    b = 1.0,

    ---@type number
    a = 1.0
}

---@overload fun(): color
---@overload fun(c: color): color
---@overload fun(c: color32): color
---@overload fun(v: v4f): color
---@overload fun(v: v3f, a: number): color
---@overload fun(r: number, g: number, b: number, a: number): color
---@return color
function color.new() end

---@return color
function color.clear() end

---@return color
function color.black() end

---@return color
function color.white() end

---@return color
function color.red() end

---@return color
function color.green() end

---@return color
function color.blue() end

---@return color
function color.yellow() end

---@return color
function color.magenta() end

---@return color
function color.cyan() end

---@param l color
---@param r color
---@return boolean
function color.approximately(l, r) end

---@param c color
---@return number
function color.minimum(c) end

---@param c color
---@return number
function color.maximum(c) end

---@param c color
---@param cmin color
---@return color
function color.minimized(c,cmin) end

---@param c color
---@param cmax color
---@return color
function color.maximized(c,cmax) end

---@param c color
---@param cmin color
---@param cmax color
---@return color
function color.clamped(c,cmin,cmax) end

---@param c color
---@return boolean
function color.contains_nan(c) end

---@type color
_G.color = _G.color or color
