local M = {}

---@param go gobject
function M:on_start(go)
    the_debug:trace("sample_08: on_start")
end

---@param go gobject
function M:on_update(go)
end

---@param go gobject
---@param type string
---@param event touchable_input_evt | touchable_mouse_evt
function M:on_event(go, type, event)
    if type == "touchable.mouse_evt" then
        the_debug:trace(string.format(
            "scene %q touched(%s %s)",
            go.named and go.named.name or "---",
            event.type,
            event.button))
    end
end

---@param go gobject
function M:on_shutdown(go)
end

return M
