---@class keyboard
local keyboard = {
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

---@param key string
---@return boolean
function keyboard:is_key_pressed(key) end

---@param key string
---@return boolean
function keyboard:is_key_just_pressed(key) end

---@param key string
---@return boolean
function keyboard:is_key_just_released(key) end

---@type keyboard
_G.the_keyboard = _G.the_keyboard or keyboard
