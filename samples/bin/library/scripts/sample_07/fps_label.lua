-- -----------------------------------------------------------------------------
--
-- private
--
-- -----------------------------------------------------------------------------

---@class fps_label_meta
---@field last_fps number

---@param meta fps_label_meta
---@param go gobject
local function update_label_text(meta, go)
    local curr_fps = the_engine.frame_rate
    if meta.last_fps ~= curr_fps then
        meta.last_fps = curr_fps
        go.label.text = "FPS: " .. curr_fps
    end
end

-- -----------------------------------------------------------------------------
--
-- meta
--
-- -----------------------------------------------------------------------------

local M = {}

---@param meta fps_label_meta
---@param go gobject
function M.on_start(meta, go)
    meta.last_fps = -1
end

---@param meta fps_label_meta
---@param go gobject
function M.on_update(meta, go)
    update_label_text(meta, go)
end

return M
