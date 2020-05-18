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

    ---@type button
    button = nil,

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

    ---@type layout
    layout = nil,

    ---@type model_renderer
    model_renderer = nil,

    ---@type named
    named = nil,

    ---@type progress
    progress = nil,

    ---@type progress_handle
    progress_handle = nil,

    ---@type renderer
    renderer = nil,

    ---@type scene
    scene = nil,

    ---@type scrollbar
    scrollbar = nil,

    ---@type scrollbar_handle
    scrollbar_handle = nil,

    ---@type slider
    slider = nil,

    ---@type slider_handle
    slider_handle = nil,

    ---@type spine_player
    spine_player = nil,

    ---@type sprite_renderer
    sprite_renderer = nil,

    ---@type toggle_group
    toggle_group = nil,

    ---@type toggle
    toggle = nil,

    ---@type touchable
    touchable = nil,

    ---@type widget
    widget = nil
}

---@param self gobject
function gobject.destroy(self) end

---@type gobject
_G.gobject = _G.gobject or gobject
