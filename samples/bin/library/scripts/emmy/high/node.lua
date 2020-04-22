---@class node
local node = {
    ---@type gobject
    owner = nil,

    ---@type t2f
    transform = t2f.identity(),

    ---@type v2f
    translation = v2f.zero(),

    ---@type number
    rotation = 0,

    ---@type v2f
    scale = v2f.unit(),

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
---@param point v4f
---@return v4f
function node.local_to_world(self, point) end

---@param self node
---@param point v4f
---@return v4f
function node.world_to_local(self, point) end

---@param self node
---@return boolean
function node.has_parent(self) end

---@param self node
---@param parent node
---@return boolean
function node.has_parent_recursive(self, parent) end

---@param self node
---@return boolean
function node.has_children(self) end

---@param self node
---@param child node
---@return boolean
function node.has_child_recursive(self, child) end

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

---@param self node
---@param index integer
---@return node
function node.child_at(self, index) end

---@param self node
---@param child node
---@return integer, boolean
function node.child_index(self, child) end

---@param self node
---@param index integer
---@return node
function node.remove_child_at(self, index) end

---@type node
_G.node = _G.node or node
