---@class rigid_body
local rigid_body = {
    ---@type boolean
    enabled = true,

    ---@type boolean
    disabled = false
}

---@overload fun(self: rigid_body)
---@param self rigid_body
function rigid_body.enable(self) end

---@overload fun(self: rigid_body)
---@param self rigid_body
function rigid_body.disable(self) end

---@type rigid_body
_G.rigid_body = _G.rigid_body or rigid_body
