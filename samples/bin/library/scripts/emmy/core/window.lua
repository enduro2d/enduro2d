---@class window
local window = {
    ---@type fun(self: window)
    hide = function(self) end,

    ---@type fun(self: window)
    show = function(self) end,

    ---@type fun(self: window)
    restore = function(self) end,

    ---@type fun(self: window)
    minimize = function(self) end,

    ---@type boolean
    enable = true,

    ---@type boolean
    visible = true,

    ---@type boolean
    focused = true,

    ---@type boolean
    minimized = true,

    ---@type boolean
    fullscreen = false,

    ---@type boolean
    cursor_hidden = false,

    ---@type v2f
    real_size = v2f.zero(),

    ---@type v2f
    virtual_size = v2f.zero(),

    ---@type v2f
    framebuffer_size = v2f.zero(),

    ---@type string
    title = "",

    ---@type boolean
    should_close = false
}

---@type window
_G.the_window = _G.the_window or window
