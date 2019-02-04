/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/scene/scene.hpp>

namespace e2d
{
    scene::scene() = default;
    scene::~scene() noexcept = default;

    scene_iptr scene::create() {
        return scene_iptr(new scene());
    }

    const node_iptr& scene::root() const noexcept {
        return root_;
    }
}
