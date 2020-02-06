---@class camera
local camera = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    input = false,

    ---@type boolean
    gizmos = false,

    ---@type integer
    depth = 0,

    ---@type camera_modes
    mode = camera.modes.flexible,

    ---@type number
    znear = 0.0,

    ---@type number
    zfar = 1000.0,

    ---@type m4f
    view = m4f.identity(),

    ---@type rect
    viewport = rect.zero(),

    ---@type m4f
    projection = m4f.identity(),

    ---@type color
    background = color.white()
}

---@class camera_modes
camera.modes = {
    manual = "manual",
    stretch = "stretch",
    flexible = "flexible",
    fixed_fit = "fixed_fit",
    fixed_crop = "fixed_crop"
}

---@overload fun(self: camera)
---@param self camera
function camera.enable(self) end

---@overload fun(self: camera)
---@param self camera
function camera.disable(self) end

---@type camera
_G.camera = _G.camera or camera
