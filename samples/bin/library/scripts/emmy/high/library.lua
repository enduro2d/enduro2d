---@class library
local library = {
}

---@param self library
---@param address string
---@return atlas_asset
function library.load_atlas(self, address) end

---@param self library
---@param address string
---@return binary_asset
function library.load_binary(self, address) end

---@param self library
---@param address string
---@return flipbook_asset
function library.load_flipbook(self, address) end

---@param self library
---@param address string
---@return font_asset
function library.load_font(self, address) end

---@param self library
---@param address string
---@return image_asset
function library.load_image(self, address) end

---@param self library
---@param address string
---@return json_asset
function library.load_json(self, address) end

---@param self library
---@param address string
---@return material_asset
function library.load_material(self, address) end

---@param self library
---@param address string
---@return mesh_asset
function library.load_mesh(self, address) end

---@param self library
---@param address string
---@return model_asset
function library.load_model(self, address) end

---@param self library
---@param address string
---@return prefab_asset
function library.load_prefab(self, address) end

---@param self library
---@param address string
---@return script_asset
function library.load_script(self, address) end

---@param self library
---@param address string
---@return shader_asset
function library.load_shader(self, address) end

---@param self library
---@param address string
---@return shape_asset
function library.load_shape(self, address) end

---@param self library
---@param address string
---@return sound_asset
function library.load_sound(self, address) end

---@param self library
---@param address string
---@return spine_asset
function library.load_spine(self, address) end

---@param self library
---@param address string
---@return sprite_asset
function library.load_sprite(self, address) end

---@param self library
---@param address string
---@return text_asset
function library.load_text(self, address) end

---@param self library
---@param address string
---@return texture_asset
function library.load_texture(self, address) end

---@param self library
---@param address string
---@return xml_asset
function library.load_xml(self, address) end

---@type library
_G.the_library = _G.the_library or library
