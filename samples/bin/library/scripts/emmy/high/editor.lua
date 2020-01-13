---@class editor
local editor = {
    ---@type gobject
    selection = nil
}

---@param self editor
---@param go gobject
function editor.select(self, go) end

---@param self editor
function editor.clear_selection(self) end

---@type editor
_G.the_editor = _G.the_editor or editor
