/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/world.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    world::world() = default;
    world::~world() noexcept = default;

    scene_iptr world::scene() noexcept {
        return scene_;
    }

    const_scene_iptr world::scene() const noexcept {
        return scene_;
    }

    ecs::registry& world::registry() noexcept {
        return registry_;
    }

    const ecs::registry& world::registry() const noexcept {
        return registry_;
    }
}
