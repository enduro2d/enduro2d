/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/world.hpp>

namespace e2d
{
    world::world() = default;
    world::~world() noexcept = default;

    ecs::registry& world::registry() noexcept {
        return registry_;
    }

    const ecs::registry& world::registry() const noexcept {
        return registry_;
    }

    gobject_iptr world::instantiate() {
        return gobject_iptr(new gobject(registry_));
    }

    gobject_iptr world::instantiate(const prefab_asset::ptr& prefab) {
        E2D_UNUSED(prefab);
        return instantiate();
    }
}
