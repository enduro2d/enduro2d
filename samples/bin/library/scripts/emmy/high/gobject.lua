---@class gobject
local gobject = {
    ---@type boolean
    alive = true,

    ---@type boolean
    valid = true,

    ---@type actor
    actor = nil,

    ---@type behaviour
    behaviour = nil,

    ---@type camera
    camera = nil,

    ---@type rect_collider
    rect_collider = nil,

    ---@type circle_collider
    circle_collider = nil,

    ---@type polygon_collider
    polygon_collider = nil,

    ---@type flipbook_player
    flipbook_player = nil,

    ---@type label
    label = nil,

    ---@type model_renderer
    model_renderer = nil,

    ---@type named
    named = nil,

    ---@type renderer
    renderer = nil,

    ---@type rigid_body
    rigid_body = nil,

    ---@type scene
    scene = nil,

    ---@type spine_player
    spine_player = nil,

    ---@type sprite_renderer
    sprite_renderer = nil,

    ---@type touchable
    touchable = nil
}

---@param self gobject
function gobject.destroy(self) end

---@type gobject
_G.gobject = _G.gobject or gobject
