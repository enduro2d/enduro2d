---@class ray3
local ray3 = {
    ---@type v3f
    origin = v3f.zero(),

    ---@type v3f
    direction = v3f.zero()
}

---@overload fun(): ray3
---@overload fun(r: ray3): ray3
---@overload fun(dx: number, dy: number, dz: number): ray2
---@overload fun(ox: number, oy: number, oz: number, dx: number, dy: number, dz: number): ray2
---@overload fun(d: v3f): ray3
---@overload fun(o: v3f, d: v3f): ray3
---@return ray3
function ray3.new(...) end

---@return ray3
function ray3.zero() end

---@return ray3
function ray3.unit_x() end

---@return ray3
function ray3.unit_y() end

---@return ray3
function ray3.unit_z() end

---@param l ray3
---@param r ray3
---@return boolean
function ray3.approximately(l, r) end

---@type ray3
_G.ray3 = _G.ray3 or ray3
