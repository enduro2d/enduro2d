---@class camera
local camera = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type integer
    depth = 0,

    ---@type rect
    viewport = rect.zero(),

    ---@type m4f
    projection = m4f.identity(),

    ---@type color
    background = color.white()
}

---@overload fun(self: camera)
---@param self camera
function camera.enable(self) end

---@overload fun(self: camera)
---@param self camera
function camera.disable(self) end

---@type camera
_G.camera = _G.camera or camera
