---@class scroll_content
local scroll_content = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: scroll_content)
---@param self scroll_content
function scroll_content.enable(self) end

---@overload fun(self: scroll_content)
---@param self scroll_content
function scroll_content.disable(self) end

---@type scroll_content
_G.scroll_content = _G.scroll_content or scroll_content
