---@class ray2
local ray2 = {
    ---@type v2f
    origin = v2f.zero(),

    ---@type v2f
    direction = v2f.zero()
}

---@overload fun(): ray2
---@overload fun(r: ray2): ray2
---@overload fun(dx: number, dy: number): ray2
---@overload fun(ox: number, oy: number, dx: number, dy: number): ray2
---@overload fun(d: v2f): ray2
---@overload fun(o: v2f, d: v2f): ray2
---@return ray2
function ray2.new(...) end

---@return ray2
function ray2.zero() end

---@return ray2
function ray2.unit_x() end

---@return ray2
function ray2.unit_y() end

---@param l ray2
---@param r ray2
---@return boolean
function ray2.approximately(l, r) end

---@param r ray2
---@return boolean
function ray2.contains_nan(r) end

---@type ray2
_G.ray2 = _G.ray2 or ray2
