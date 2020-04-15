---@class widget
local widget = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type boolean
    dirty = false,

    ---@type v2f
    size = v2f.zero(),

    ---@type v2f
    margin = v2f.zero(),

    ---@type v2f
    padding = v2f.zero()
}

---@overload fun(self: widget)
---@param self widget
function widget.enable(self) end

---@overload fun(self: widget)
---@param self widget
function widget.disable(self) end

---@type widget
_G.widget = _G.widget or widget
