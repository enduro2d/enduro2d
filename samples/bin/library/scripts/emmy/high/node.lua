---@class node
local node = {
    ---@type t3f
    transform = t3f.identity(),

    ---@type v3f
    translation = v3f.zero(),

    ---@type q4f
    rotation = q4f.identity(),

    ---@type v3f
    scale = v3f.unit(),

    ---@type m4f
    local_matrix = m4f.identity(),

    ---@type m4f
    world_matrix = m4f.identity(),

    ---@type node
    root = nil,

    ---@type node
    parent = nil,

    ---@type integer
    child_count = 0,

    ---@type integer
    child_count_recursive = 0,

    ---@type node
    first_child = nil,

    ---@type node
    last_child = nil,

    ---@type node
    prev_sibling = nil,

    ---@type node
    next_sibling = nil
}

---@param self node
---@return boolean
function node.remove_from_parent(self) end

---@param self node
---@return integer
function node.remove_all_children(self) end

---@param self node
---@param child node
---@return boolean
function node.add_child(self, child) end

---@param self node
---@param child node
---@return boolean
function node.add_child_to_back(self, child) end

---@param self node
---@param child node
---@return boolean
function node.add_child_to_front(self, child) end

---@param self node
---@param before node
---@param child node
---@return boolean
function node.add_child_before(self, before, child) end

---@param self node
---@param after node
---@param child node
---@return boolean
function node.add_child_after(self, after, child) end

---@param self node
---@param sibling node
---@return boolean
function node.add_sibling_before(self, sibling) end

---@param self node
---@param sibling node
---@return boolean
function node.add_sibling_after(self, sibling) end

---@param self node
---@param child node
---@return boolean
function node.remove_child(self, child) end

---@param self node
---@return boolean
function node.send_backward(self) end

---@param self node
---@return boolean
function node.bring_to_back(self) end

---@param self node
---@return boolean
function node.send_forward(self) end

---@param self node
---@return boolean
function node.bring_to_front(self) end

---@type node
_G.node = _G.node or node
