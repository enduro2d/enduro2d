/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/world.hpp>

namespace e2d
{
    world::~world() noexcept {
        while ( !gobjects_.empty() ) {
            destroy_instance(gobjects_.begin()->second);
        }
    }

    ecs::registry& world::registry() noexcept {
        return registry_;
    }

    const ecs::registry& world::registry() const noexcept {
        return registry_;
    }

    gobject_iptr world::instantiate() {
        auto instance = make_intrusive<gobject>(registry_);
        gobjects_.emplace(instance->entity().id(), instance);
        return instance;
    }

    gobject_iptr world::instantiate(const prefab& prefab) {
        auto instance = make_intrusive<gobject>(registry_, prefab.prototype());
        gobjects_.emplace(instance->entity().id(), instance);
        return instance;
    }

    void world::destroy_instance(const gobject_iptr& inst) noexcept {
        if ( inst ) {
            inst->entity().remove_all_components();
            gobjects_.erase(inst->entity().id());
        }
    }
}
