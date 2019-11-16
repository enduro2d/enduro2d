-- -----------------------------------------------------------------------------
--
-- private
--
-- -----------------------------------------------------------------------------

---@class gnome_meta
---@field life_time number

---@param meta gnome_meta
---@param go gobject
local function update_life_time(meta, go)
    meta.life_time = meta.life_time - the_engine.delta_time
    if meta.life_time <= 0 then
        go:destroy()
    end
end

---@param meta gnome_meta
---@param go gobject
local function update_gnome_rotation(meta, go)
    go.actor.node.rotation = q4f.make_from_euler_angles(
        radf.new(),
        radf.new(the_engine.time),
        radf.new())
end

-- -----------------------------------------------------------------------------
--
-- meta
--
-- -----------------------------------------------------------------------------

local M = {}

---@param meta gnome_meta
---@param go gobject
function M.on_start(meta, go)
    meta.life_time = 5
end

---@param meta gnome_meta
---@param go gobject
function M.on_update(meta, go)
    update_life_time(meta, go)
    update_gnome_rotation(meta, go)
end

return M
