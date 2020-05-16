---@class label
local label = {
    ---@type boolean
    dirty = false,

    ---@type string
    text = "",

    ---@type font_asset
    font = nil,

    ---@type color32
    tint = color32.white(),

    ---@type label_wraps
    wrap = label.wraps.no_wrap,

    ---@type label_haligns
    halign = label.haligns.center,

    ---@type label_valigns
    valign = label.valigns.center,

    ---@type number
    leading = 1,

    ---@type number
    tracking = 0,

    ---@type number
    glyph_dilate = 0,

    ---@type number
    outline_width = 0,

    ---@type color32
    outline_color = color32.white()
}

---@class label_wraps
label.wraps = {
    no_wrap = "no_wrap",
    wrap_by_chars = "wrap_by_chars",
    wrap_by_spaces = "wrap_by_spaces"
}

---@class label_haligns
label.haligns = {
    left = "left",
    center = "center",
    right = "right"
}

---@class label_valigns
label.valigns = {
    top = "top",
    center = "center",
    bottom = "bottom"
}

---@type label
_G.label = _G.label or label
