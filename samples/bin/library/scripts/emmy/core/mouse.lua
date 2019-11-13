---@class mouse
local mouse = {
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

---@param self mouse
---@param button string
---@return boolean
function mouse.is_button_pressed(self, button) end

---@param self mouse
---@param button string
---@return boolean
function mouse.is_button_just_pressed(self, button) end

---@param self mouse
---@param button string
---@return boolean
function mouse.is_button_just_released(self, button) end

---@type mouse
_G.the_mouse = _G.the_mouse or mouse
