---@class str_hash
local str_hash = {
    ---@type boolean
    empty = true,

    ---@type number
    hash = 0
}

---@overload fun(): str_hash
---@overload fun(s: string): str_hash
---@return str_hash
function str_hash.new() end

---@overload fun(s: str_hash)
---@param s str_hash
function str_hash.clear(s) end

---@type str_hash
_G.str_hash = _G.str_hash or str_hash
