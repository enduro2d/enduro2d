---@class widget
local widget = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false,

    ---@type v2f
    size = v2f.zero(),

    ---@type v2f
    pivot = v2f.unit() * 0.5
}

---@overload fun(self: widget)
---@param self widget
function widget.enable(self) end

---@overload fun(self: widget)
---@param self widget
function widget.disable(self) end

---@type widget
_G.widget = _G.widget or widget
