/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/atlas_asset.hpp"
#include "assets/binary_asset.hpp"
#include "assets/flipbook_asset.hpp"
#include "assets/font_asset.hpp"
#include "assets/image_asset.hpp"
#include "assets/json_asset.hpp"
#include "assets/material_asset.hpp"
#include "assets/mesh_asset.hpp"
#include "assets/model_asset.hpp"
#include "assets/prefab_asset.hpp"
#include "assets/shader_asset.hpp"
#include "assets/shape_asset.hpp"
#include "assets/sound_asset.hpp"
#include "assets/sprite_asset.hpp"
#include "assets/text_asset.hpp"
#include "assets/texture_asset.hpp"
#include "assets/xml_asset.hpp"

#include "components/actor.hpp"
#include "components/camera.hpp"
#include "components/colliders.hpp"
#include "components/commands.hpp"
#include "components/disabled.hpp"
#include "components/events.hpp"
#include "components/flipbook_player.hpp"
#include "components/label.hpp"
#include "components/layout.hpp"
#include "components/model_renderer.hpp"
#include "components/named.hpp"
#include "components/renderer.hpp"
#include "components/scene.hpp"
#include "components/sprite_renderer.hpp"
#include "components/touchable.hpp"
#include "components/widget.hpp"

#include "resources/atlas.hpp"
#include "resources/flipbook.hpp"
#include "resources/model.hpp"
#include "resources/prefab.hpp"
#include "resources/sprite.hpp"

#include "systems/camera_system.hpp"
#include "systems/flipbook_system.hpp"
#include "systems/frame_system.hpp"
#include "systems/gizmos_system.hpp"
#include "systems/label_system.hpp"
#include "systems/layout_system.hpp"
#include "systems/render_system.hpp"
#include "systems/touch_system.hpp"
#include "systems/world_system.hpp"

#include "address.hpp"
#include "asset.hpp"
#include "asset.inl"
#include "editor.hpp"
#include "factory.hpp"
#include "factory.inl"
#include "gobject.hpp"
#include "inspector.hpp"
#include "inspector.inl"
#include "library.hpp"
#include "library.inl"
#include "node.hpp"
#include "node.inl"
#include "starter.hpp"
#include "world.hpp"
