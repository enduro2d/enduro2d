---@class engine
local engine = {
    ---@type number
    time = 0,

    ---@type number
    delta_time = 0,

    ---@type number
    frame_rate = 0,

    ---@type number
    frame_count = 0,

    ---@type number
    realtime_time = 0
}

---@type engine
_G.the_engine = _G.the_engine or engine
