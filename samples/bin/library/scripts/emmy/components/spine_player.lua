---@class spine_player
local spine_player = {
    ---@type spine_asset
    spine = nil
}

---@param self spine_player
---@param name string
---@return boolean
function spine_player.skin(self, name) end

---@param self spine_player
---@param slot string
---@param name string
---@return boolean
function spine_player.attachment(self, slot, name) end

---@param self spine_player
---@param name string
---@return boolean
function spine_player.has_skin(self, name) end

---@param self spine_player
---@param name string
---@return boolean
function spine_player.has_animation(self, name) end

---@param self spine_player
---@param cmd spine_player_command
function spine_player.add_command(self, cmd) end

-- -----------------------------------------------------------------------------
--
-- events
--
-- -----------------------------------------------------------------------------

---@alias spine_player_event spine_player_custom_evt | spine_player_end_evt | spine_player_complete_evt

--
-- custom_evt
--

---@class spine_player_custom_evt
spine_player.custom_evt = {
    ---@type string
    name = "",

    ---@type integer
    int_value = 0,

    ---@type number
    float_value = 0,

    ---@type string
    string_value = ""
}

---@overload fun(): spine_player_custom_evt
---@overload fun(name: string): spine_player_custom_evt
---@return spine_player_custom_evt
function spine_player.custom_evt.new(...) end

--
-- end_evt
--

---@class spine_player_end_evt
spine_player.end_evt = {
    ---@type string
    message = ""
}

---@overload fun(): spine_player_end_evt
---@overload fun(message: string): spine_player_end_evt
---@return spine_player_end_evt
function spine_player.end_evt.new(...) end

--
-- complete
--

---@class spine_player_complete_evt
spine_player.complete_evt = {
    ---@type string
    message = ""
}

---@overload fun(): spine_player_complete_evt
---@overload fun(message: string): spine_player_complete_evt
---@return spine_player_complete_evt
function spine_player.complete_evt.new(...) end

-- -----------------------------------------------------------------------------
--
-- commands
--
-- -----------------------------------------------------------------------------

---@alias spine_player_command spine_player_clear_track_cmd | spine_player_set_anim_cmd | spine_player_add_anim_cmd | spine_player_set_empty_anim_cmd | spine_player_add_empty_anim_cmd

--
-- clear_track_cmd
--

---@class spine_player_clear_track_cmd
spine_player.clear_track_cmd = {
    ---@type integer
    track = 0
}

---@param track integer
---@return spine_player_clear_track_cmd
function spine_player.clear_track_cmd.new(track) end

--
-- set_anim_cmd
--

---@class spine_player_set_anim_cmd
spine_player.set_anim_cmd = {
    ---@type integer
    track = 0,

    ---@type string
    name = "",

    ---@type boolean
    loop = false,

    ---@type string
    end_message = "",

    ---@type string
    complete_message = ""
}

---@param track integer
---@param name string
---@return spine_player_set_anim_cmd
function spine_player.set_anim_cmd.new(track, name) end

--
-- add_anim_cmd
--

---@class spine_player_add_anim_cmd
spine_player.add_anim_cmd = {
    ---@type integer
    track = 0,

    ---@type string
    name = "",

    ---@type boolean
    loop = false,

    ---@type number
    delay = 0,

    ---@type string
    end_message = "",

    ---@type string
    complete_message = ""
}

---@param track integer
---@param name string
---@return spine_player_add_anim_cmd
function spine_player.add_anim_cmd.new(track, name) end

--
-- set_empty_anim_cmd
--

---@class spine_player_set_empty_anim_cmd
spine_player.set_empty_anim_cmd = {
    ---@type integer
    track = 0,

    ---@type number
    mix_duration = 0,

    ---@type string
    end_message = "",

    ---@type string
    complete_message = ""
}

---@param track integer
---@return spine_player_set_empty_anim_cmd
function spine_player.set_empty_anim_cmd.new(track) end

--
-- add_empty_anim_cmd
--

---@class spine_player_add_empty_anim_cmd
spine_player.add_empty_anim_cmd = {
    ---@type integer
    track = 0,

    ---@type number
    delay = 0,

    ---@type number
    mix_duration = 0,

    ---@type string
    end_message = "",

    ---@type string
    complete_message = ""
}

---@param track integer
---@return spine_player_add_empty_anim_cmd
function spine_player.add_empty_anim_cmd.new(track) end

-- -----------------------------------------------------------------------------
--
-- globals
--
-- -----------------------------------------------------------------------------

---@type spine_player
_G.spine_player = _G.spine_player or spine_player
