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
---@param event touchable_base_evt | touchable_mouse_evt | touchable_hover_evt
function M:on_event(go, type, event)
    if type == "touchable.mouse_evt" then
        the_debug:trace(string.format(
            "scene %q mouse_evt(%s %s) - %s",
            go.named and go.named.name or "---",
            event.type,
            event.button,
            go == event.target and "self" or "other"))
    end

    if type == "touchable.hover_evt" then
        the_debug:trace(string.format(
            "scene %q hover_evt(%s) - %s",
            go.named and go.named.name or "---",
            event.type,
            go == event.target and "self" or "other"))
    end
end

---@param go gobject
function M:on_shutdown(go)
end

return M
