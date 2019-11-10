---@class color32
local color32 = {
    ---@type number
    r = 255,

    ---@type number
    g = 255,

    ---@type number
    b = 255,

    ---@type number
    a = 255
}

---@overload fun(): color32
---@overload fun(c: color): color32
---@overload fun(c: color32): color32
---@overload fun(v: v4f): color32
---@overload fun(v: v3f, a: number): color32
---@overload fun(r: number, g: number, b: number, a: number): color32
---@return color32
function color32.new() end

---@return color32
function color32.clear() end

---@return color32
function color32.black() end

---@return color32
function color32.white() end

---@return color32
function color32.red() end

---@return color32
function color32.green() end

---@return color32
function color32.blue() end

---@return color32
function color32.yellow() end

---@return color32
function color32.magenta() end

---@return color32
function color32.cyan() end

---@param l color32
---@param r color32
---@return boolean
function color32.approximately(l, r) end

---@param c color32
---@return number
function color32.minimum(c) end

---@param c color32
---@return number
function color32.maximum(c) end

---@param c color32
---@param cmin color32
---@return color32
function color32.minimized(c,cmin) end

---@param c color32
---@param cmax color32
---@return color32
function color32.maximized(c,cmax) end

---@param c color32
---@param cmin color32
---@param cmax color32
---@return color32
function color32.clamped(c,cmin,cmax) end

---@param c color32
---@return boolean
function color32.contains_nan(c) end

---@type color32
_G.color32 = _G.color32 or color32
