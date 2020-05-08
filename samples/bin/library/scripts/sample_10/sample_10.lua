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
    if type == "touchable.mouse_move_evt" then
        the_debug:trace(string.format("event: %q\n-->target: %q\n-->local_point: %s\n-->world_point: %s",
            type,
            event.target.named and event.target.named.name or "[unnamed]",
            event.local_point,
            event.world_point))
    end

    if type == "touchable.mouse_scroll_evt" then
        the_debug:trace(string.format("event: %q\n-->delta: %s\n-->target: %q\n-->local_point: %s\n-->world_point: %s",
            type,
            event.delta,
            event.target.named and event.target.named.name or "[unnamed]",
            event.local_point,
            event.world_point))
    end

    if type == "touchable.mouse_button_evt" then
        the_debug:trace(string.format("event: %q\n-->type: %q\n-->button: %q\n-->target: %q\n-->local_point: %s\n-->world_point: %s",
            type,
            event.type,
            event.button,
            event.target.named and event.target.named.name or "[unnamed]",
            event.local_point,
            event.world_point))
    end
end

---@param go gobject
function M:on_shutdown(go)
end

return M
