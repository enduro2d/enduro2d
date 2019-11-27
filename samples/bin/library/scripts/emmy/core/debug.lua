---@class debug
local debug = {
}

---@param self debug
---@param message string
function debug.trace(self, message) end

---@param self debug
---@param message string
function debug.warning(self, message) end

---@param self debug
---@param message string
function debug.error(self, message) end

---@param self debug
---@param message string
function debug.fatal(self, message) end

---@type debug
_G.the_debug = _G.the_debug or debug
