local M = {}

---@param go gobject
function M:on_start(go)
    the_debug:trace("sample_10: on_start")
end

---@param go gobject
function M:on_update(go)
end

---@param go gobject
---@param type string
---@param event touchable_event
function M:on_event(go, type, event)
    if type == "touchable.mouse_evt" then
        the_debug:trace(string.format("event: %q, type: %q, button: %q, target: %q",
            type,
            event.type,
            event.button,
            event.target.named and event.target.named.name or "[unnamed]"))
    end
end

---@param go gobject
function M:on_shutdown(go)
end

return M
