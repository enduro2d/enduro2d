/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/world.hpp>

#include <enduro2d/high/node.hpp>
#include <enduro2d/high/components/actor.hpp>

namespace e2d
{
    world::~world() noexcept {
        while ( !gobjects_.empty() ) {
            destroy_instance(gobjects_.begin()->second, true);
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

    void world::destroy_instance(const gobject_iptr& inst, bool recursive) noexcept {
        if ( recursive ) {
            node_iptr inst_n = inst && inst->get_component<actor>().exists()
                ? inst->get_component<actor>().get().node()
                : nullptr;
            if ( inst_n ) {
                inst_n->for_each_child([this](const node_iptr& child_n){
                    destroy_instance(child_n->owner(), true);
                });
            }
        }
        if ( inst ) {
            inst->entity().remove_all_components();
            gobjects_.erase(inst->entity().id());
        }
    }
}
