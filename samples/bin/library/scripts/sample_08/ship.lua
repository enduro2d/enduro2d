-- -----------------------------------------------------------------------------
--
-- private
--
-- -----------------------------------------------------------------------------

---@class ship_meta

---@param meta ship_meta
---@param go gobject
local function update_ship_rotation(meta, go)
    local time = the_engine.time
    go.actor.node.rotation = time
end

-- -----------------------------------------------------------------------------
--
-- meta
--
-- -----------------------------------------------------------------------------

local M = {}

---@param meta ship_meta
---@param go gobject
function M.on_start(meta, go)
    meta.life_time = 5
end

---@param meta ship_meta
---@param go gobject
function M.on_update(meta, go)
    update_ship_rotation(meta, go)
end

return M
