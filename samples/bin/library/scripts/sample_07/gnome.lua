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
    local dt = the_engine.delta_time
    meta.life_time = meta.life_time - dt
    if meta.life_time <= 0 then
        go:destroy()
    end
end

---@param meta gnome_meta
---@param go gobject
local function update_gnome_rotation(meta, go)
    local time = the_engine.time
    go.renderer.rotation = v3f.new(0, time, 0)
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
