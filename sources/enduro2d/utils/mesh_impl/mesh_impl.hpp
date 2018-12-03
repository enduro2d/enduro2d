/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/mesh.hpp>
#include <enduro2d/utils/buffer.hpp>

namespace e2d { namespace meshes { namespace impl
{
    bool try_load_mesh_assimp(mesh& dst, const buffer& src) noexcept;
}}}
