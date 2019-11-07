---@class keyboard
local keyboard = {
    ---@type fun(self: keyboard, key : string): boolean
    is_key_pressed = function(self, key) return false end,

    ---@type fun(self: keyboard, key : string): boolean
    is_key_just_pressed = function(self, key) return false end,

    ---@type fun(self: keyboard, key : string): boolean
    is_key_just_released = function(self, key) return false end,

    ---@type string
    input_text = "",

    ---@type boolean
    is_any_key_pressed = false,

    ---@type boolean
    is_any_key_just_pressed = false,

    ---@type boolean
    is_any_key_just_released = false,

    ---@type string[]
    pressed_keys = {},

    ---@type string[]
    just_pressed_keys = {},

    ---@type string[]
    just_released_keys = {}
}

---@type keyboard
_G.the_keyboard = _G.the_keyboard or keyboard
