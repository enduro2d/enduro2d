-- -----------------------------------------------------------------------------
--
-- private
--
-- -----------------------------------------------------------------------------

---@class panel_meta

-- -----------------------------------------------------------------------------
--
-- meta
--
-- -----------------------------------------------------------------------------

local M = {}

---@param meta panel_meta
---@param go gobject
function M.on_start(meta, go)
end

---@param meta panel_meta
---@param go gobject
function M.on_update(meta, go)
end

---@param go gobject
---@param type string
---@param event touchable_base_evt | touchable_click_evt
function M:on_event(go, type, event)
    if type == "touchable.click_evt" then
        local offset = v2f.new(math.random(-15,15), math.random(-15,15))
        go.actor.node.translation = go.actor.node.translation + offset
    end
end

return M
