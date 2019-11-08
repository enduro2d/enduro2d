---@class window
local window = {
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

function window:hide() end

function window:show() end

function window:restore() end

function window:minimize() end

---@type window
_G.the_window = _G.the_window or window
