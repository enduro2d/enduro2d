---@class debug
local debug = {
    ---@type fun(self: debug, message: string)
    trace = function(self, message) end,

    ---@type fun(self: debug, message: string)
    warning = function(self, message) end,

    ---@type fun(self: debug, message: string)
    error = function(self, message) end,

    ---@type fun(self: debug, message: string)
    fatal = function(self, message) end
}

---@type debug
_G.the_debug = _G.the_debug or debug
