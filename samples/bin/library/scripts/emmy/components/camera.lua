---@class camera
local camera = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type integer
    depth = 0,

    ---@type number
    znear = 0.0,

    ---@type number
    zfar = 1000.0,

    ---@type rect
    viewport = rect.zero(),

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
