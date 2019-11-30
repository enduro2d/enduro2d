---@class world
local world = {
}

---@overload fun(prefab: prefab): gobject
---@overload fun(prefab: prefab, parent: node): gobject
---@overload fun(prefab: prefab, parent: node, transform: t2f): gobject
---@return gobject
function world:instantiate(...) end

---@type world
_G.the_world = _G.the_world or world
