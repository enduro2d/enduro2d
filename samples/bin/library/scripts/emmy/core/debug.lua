---@class debug
local debug = {
}

---@param message string
function debug:trace(message) end

---@param message string
function debug:warning(message) end

---@param message string
function debug:error(message) end

---@param message string
function debug:fatal(message) end

---@type debug
_G.the_debug = _G.the_debug or debug
