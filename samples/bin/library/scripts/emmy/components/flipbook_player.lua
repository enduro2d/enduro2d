---@class flipbook_player
local flipbook_player = {
    ---@type number
    time = 0,

    ---@type number
    speed = 1,

    ---@type boolean
    looped = false,

    ---@type boolean
    stopped = true,

    ---@type boolean
    playing = false,

    ---@type str_hash
    sequence = str_hash.new(),

    ---@type flipbook_asset
    flipbook = nil
}

---@overload fun(self: flipbook_player, time: number)
---@overload fun(self: flipbook_player, sequence: string)
---@overload fun(self: flipbook_player, sequence: str_hash)
---@param self flipbook_player
function flipbook_player.stop(self, ...) end

---@overload fun(self: flipbook_player, time: number)
---@overload fun(self: flipbook_player, sequence: string)
---@overload fun(self: flipbook_player, sequence: str_hash)
---@param self flipbook_player
function flipbook_player.play(self, ...) end

---@type flipbook_player
_G.flipbook_player = _G.flipbook_player or flipbook_player
