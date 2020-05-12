---@class touchable
local touchable = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    pushing = false,

    ---@type boolean
    dragging = false,

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
-- touchable_mouse_drag_evt
--

---@class touchable_mouse_drag_evt : touchable_base_evt
local touchable_mouse_drag_evt = {
    ---@type string
    type = "start",

    ---@type v2f
    local_point = v2f.zero(),

    --@type v2f
    world_point = v2f.zero()
}
touchable.touchable_mouse_drag_evt = touchable_mouse_drag_evt

--
-- touchable_mouse_move_evt
--

---@class touchable_mouse_move_evt : touchable_base_evt
local touchable_mouse_move_evt = {
    ---@type v2f
    local_point = v2f.zero(),

    --@type v2f
    world_point = v2f.zero()
}
touchable.touchable_mouse_move_evt = touchable_mouse_move_evt

--
-- touchable_mouse_hover_evt
--

---@class touchable_mouse_hover_evt : touchable_base_evt
local touchable_mouse_hover_evt = {
    ---@type string
    type = "over"
}
touchable.touchable_mouse_hover_evt = touchable_mouse_hover_evt

--
-- touchable_mouse_scroll_evt
--

---@class touchable_mouse_scroll_evt : touchable_base_evt
local touchable_mouse_scroll_evt = {
    ---@type v2f
    delta = v2f.zero(),

    ---@type v2f
    local_point = v2f.zero(),

    --@type v2f
    world_point = v2f.zero()
}
touchable.touchable_mouse_scroll_evt = touchable_mouse_scroll_evt

--
-- touchable_mouse_button_evt
--

---@class touchable_mouse_button_evt : touchable_base_evt
local touchable_mouse_button_evt = {
    ---@type string
    type = "pressed",

    ---@type string
    button = "left",

    ---@type v2f
    local_point = v2f.zero(),

    --@type v2f
    world_point = v2f.zero()
}
touchable.touchable_mouse_button_evt = touchable_mouse_button_evt

---@alias touchable_event touchable_mouse_drag_evt | touchable_mouse_move_evt | touchable_mouse_hover_evt | touchable_mouse_scroll_evt | touchable_mouse_button_evt

-- -----------------------------------------------------------------------------
--
-- globals
--
-- -----------------------------------------------------------------------------

---@type touchable
_G.touchable = _G.touchable or touchable
