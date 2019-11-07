---@class mouse
local mouse = {
    ---@type fun(self: mouse, button : string): boolean
    is_button_pressed = function(self, button) return false end,

    ---@type fun(self: mouse, button : string): boolean
    is_button_just_pressed = function(self, button) return false end,

    ---@type fun(self: mouse, button : string): boolean
    is_button_just_released = function(self, button) return false end,

    ---@type v2f
    cursor_pos = v2f.zero(),

    ---@type v2f
    scroll_delta = v2f.zero(),

    ---@type boolean
    is_any_button_pressed = false,

    ---@type boolean
    is_any_button_just_pressed = false,

    ---@type boolean
    is_any_button_just_released = false,

    ---@type string[]
    pressed_buttons = {},

    ---@type string[]
    just_pressed_buttons = {},

    ---@type string[]
    just_released_buttons = {}
}

---@type mouse
_G.the_mouse = _G.the_mouse or mouse
