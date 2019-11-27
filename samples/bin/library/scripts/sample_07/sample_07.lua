local M = {}

---@param go gobject
function M:on_start(go)
    the_debug:trace("sample_07: on_start")
end

---@param go gobject
function M:on_update(go)
end

---@param go gobject
---@param type string
---@param event any
function M:on_event(go, type, event)
end

---@param go gobject
function M:on_shutdown(go)
end

return M
