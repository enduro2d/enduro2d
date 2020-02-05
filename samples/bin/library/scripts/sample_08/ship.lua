-- -----------------------------------------------------------------------------
--
-- private
--
-- -----------------------------------------------------------------------------

---@class ship_meta

-- -----------------------------------------------------------------------------
--
-- meta
--
-- -----------------------------------------------------------------------------

local M = {}

---@param meta ship_meta
---@param go gobject
function M.on_start(meta, go)
end

---@param meta ship_meta
---@param go gobject
function M.on_update(meta, go)
end

---@param go gobject
---@param type string
---@param event touchable_mouse_evt
function M:on_event(go, type, event)
    if type == "touchable.mouse_evt" then
        the_debug:trace(string.format(
            "ship %q touched(%s %s) - %s",
            go.named and go.named.name or "---",
            event.type,
            event.button,
            go == event.target and "self" or "other"))
    end
end

return M
