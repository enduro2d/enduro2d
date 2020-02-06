/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/mesh.hpp>
#include <enduro2d/utils/buffer.hpp>
#include <enduro2d/utils/buffer_view.hpp>

namespace e2d::meshes::impl
{
    bool load_mesh_e2d(mesh& dst, buffer_view src);
}
