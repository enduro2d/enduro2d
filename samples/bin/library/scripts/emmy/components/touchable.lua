---@class touchable
local touchable = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    pushing = false,

    ---@type boolean
    hovering = false,

    ---@type boolean
    clicked = false,

    ---@type boolean
    pressed = false,

    ---@type boolean
    released = false,

    ---@type boolean
    hover_over = false,

    ---@type boolean
    hover_out = false,

    ---@type boolean
    hover_enter = false,

    ---@type boolean
    hover_leave = false,

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
-- touchable_base_evt
--

---@class touchable_base_evt
local touchable_base_evt = {
    ---@type gobject
    target = nil,

    ---@type boolean
    bubbling = false
}
touchable.touchable_base_evt = touchable_base_evt

--
-- touchable_click_evt
--

---@class touchable_click_evt : touchable_base_evt
local touchable_click_evt = {
}
touchable.touchable_click_evt = touchable_click_evt

--
-- touchable_mouse_evt
--

---@class touchable_mouse_evt : touchable_base_evt
local touchable_mouse_evt = {
    ---@type string
    type = "pressed",

    ---@type string
    button = "left"
}
touchable.touchable_mouse_evt = touchable_mouse_evt

--
-- touchable_touch_evt
--

---@class touchable_touch_evt : touchable_base_evt
local touchable_touch_evt = {
    ---@type string
    type = "pressed",

    ---@type integer
    finger = 0
}
touchable.touchable_touch_evt = touchable_touch_evt

--
-- touchable_hover_evt
--

---@class touchable_hover_evt : touchable_base_evt
local touchable_hover_evt = {
    ---@type string
    type = "over"
}
touchable.touchable_hover_evt = touchable_hover_evt

---@alias touchable_event touchable_click_evt | touchable_mouse_evt | touchable_touch_evt | touchable_hover_evt

-- -----------------------------------------------------------------------------
--
-- globals
--
-- -----------------------------------------------------------------------------

---@type touchable
_G.touchable = _G.touchable or touchable
