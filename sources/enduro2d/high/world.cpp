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
        auto instance = gobject_iptr(new gobject(registry_));
        intrusive_ptr_add_ref(instance.get());
        gobjects_.push_back(*instance);
        return instance;
    }

    gobject_iptr world::instantiate(const prefab_asset::ptr& prefab) {
        auto instance = prefab
            ? gobject_iptr(new gobject(registry_, prefab->content().prototype()))
            : gobject_iptr(new gobject(registry_));
        intrusive_ptr_add_ref(instance.get());
        gobjects_.push_back(*instance);
        return instance;
    }

    gobject_iptr world::instantiate(const const_gobject_iptr& source) {
        auto instance = source
            ? gobject_iptr(new gobject(registry_, *source))
            : gobject_iptr(new gobject(registry_));
        intrusive_ptr_add_ref(instance.get());
        gobjects_.push_back(*instance);
        return instance;
    }
}
