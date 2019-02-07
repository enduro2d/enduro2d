/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../core/_all.hpp"

#include <3rdparty/ecs.hpp/ecs.hpp>

namespace e2d
{
    namespace ecs
    {
        using namespace ecs_hpp;
    }
}

namespace e2d
{
    class node;
    using node_iptr = intrusive_ptr<node>;
    using const_node_iptr = intrusive_ptr<const node>;

    class scene;
    using scene_iptr = intrusive_ptr<scene>;
    using const_scene_iptr = intrusive_ptr<const scene>;

    class actor;
    using actor_iptr = intrusive_ptr<actor>;
    using const_actor_iptr = intrusive_ptr<const actor>;

    class node_children_ilist_tag {};
    using node_children = intrusive_list<node, node_children_ilist_tag>;
}

namespace e2d
{
    class camera;
    class drawable;
    class sprite;
    class transform2d;
    class transform3d;

    class actor;
    class node;
    class scene;

    class render_system;
    class sprite_system;

    class asset;
    class library;

    class text_asset;
    class mesh_asset;
    class image_asset;
    class binary_asset;
    class shader_asset;
    class texture_asset;
    class material_asset;

    template < typename Asset, typename Content >
    class content_asset;

    template < typename T >
    class asset_cache;
    class asset_cache_base;

    class starter;
    class world;
}
