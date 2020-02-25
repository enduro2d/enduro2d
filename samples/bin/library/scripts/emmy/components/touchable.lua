---@class touchable
local touchable = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    hover = false,

    ---@type boolean
    pressed = false,

    ---@type boolean
    released = false,

    ---@type boolean
    bubbling = true,

    ---@type boolean
    capturing = true
}

---@overload fun(self: touchable)
---@param self touchable
function touchable.enable(self) end

---@overload fun(self: touchable)
---@param self touchable
function touchable.disable(self) end

-- -----------------------------------------------------------------------------
--
-- events
--
-- -----------------------------------------------------------------------------

--
-- input_evt
--

---@class touchable_base_evt
touchable.input_evt = {
    ---@type gobject
    target = nil,

    ---@type boolean
    bubbling = true,

    ---@type boolean
    capturing = true
}

--
-- mouse_evt
--

---@class touchable_mouse_evt : touchable_base_evt
touchable.mouse_evt = {
    ---@type string
    type = "pressed",

    ---@type string
    button = "left"
}

--
-- touch_evt
--

---@class touchable_touch_evt : touchable_base_evt
touchable.touch_evt = {
    ---@type string
    type = "pressed",

    ---@type integer
    finger = 0
}

---@alias touchable_event touchable_mouse_evt | touchable_touch_evt

-- -----------------------------------------------------------------------------
--
-- globals
--
-- -----------------------------------------------------------------------------

---@type touchable
_G.touchable = _G.touchable or touchable
