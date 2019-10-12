/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/world.hpp>

#include <enduro2d/high/components/actor.hpp>

namespace
{
    using namespace e2d;

    class gobject_state final : public gobject::state {
    public:
        gobject_state(ecs::registry& registry)
        : entity_(registry.create_entity()) {}

        gobject_state(ecs::registry& registry, const ecs::prototype& proto)
        : entity_(registry.create_entity(proto)) {}

        ~gobject_state() final {
            destroy();
        }

        void destroy() noexcept {
            if ( valid() ) {
                entity_.destroy();
                valid_ = false;
            }
        }

        bool valid() const noexcept final {
            return valid_;
        }

        ecs::entity raw_entity() noexcept final {
            E2D_ASSERT(valid());
            return entity_;
        }

        ecs::const_entity raw_entity() const noexcept final {
            E2D_ASSERT(valid());
            return entity_;
        }
    private:
        bool valid_{true};
        ecs::entity entity_;
    };
}

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

    gobject world::instantiate() {
        gobject inst{make_intrusive<gobject_state>(registry_)};
        gobjects_.emplace(inst.raw_entity().id(), inst);

        try {
            auto n = node::create(inst);
            gcomponent<actor> inst_a{inst};
            if ( inst_a && inst_a->node() ) {
                n->transform(inst_a->node()->transform());
            }
            inst_a.assign(n);
        } catch (...) {
            destroy_instance(inst);
            throw;
        }

        return inst;
    }

    gobject world::instantiate(const prefab& prefab) {
        gobject inst{make_intrusive<gobject_state>(registry_, prefab.prototype())};
        gobjects_.emplace(inst.raw_entity().id(), inst);

        try {
            auto n = node::create(inst);
            gcomponent<actor> inst_a{inst};
            if ( inst_a && inst_a->node() ) {
                n->transform(inst_a->node()->transform());
            }
            inst_a.assign(n);
        } catch (...) {
            destroy_instance(inst);
            throw;
        }

        try {
            for ( const auto& child_prefab : prefab.children() ) {
                auto child = instantiate(child_prefab);
                try {
                    gcomponent<actor> inst_a{inst};
                    gcomponent<actor> child_a{child};
                    inst_a->node()->add_child(child_a->node());
                } catch (...) {
                    destroy_instance(child);
                    throw;
                }
            }
        } catch (...) {
            destroy_instance(inst);
            throw;
        }

        return inst;
    }

    gobject world::instantiate(const gobject& parent) {
        gobject go = instantiate();
        if ( go && parent ) {
            gcomponent<actor> parent_a{parent};
            if ( !parent_a ) {
                parent_a.assign(node::create(parent));
            }
            if ( !parent_a->node() ) {
                parent_a->node(node::create(parent));
            }
            parent_a->node()->add_child(gcomponent<actor>{go}->node());
        }
        return go;
    }

    gobject world::instantiate(const node_iptr& parent) {
        gobject go = instantiate();
        if ( go && parent ) {
            parent->add_child(gcomponent<actor>{go}->node());
        }
        return go;
    }

    gobject world::instantiate(const prefab& prefab, const gobject& parent) {
        gobject go = instantiate(prefab);
        if ( go && parent ) {
            gcomponent<actor> parent_a{parent};
            if ( !parent_a ) {
                parent_a.assign(node::create(parent));
            }
            if ( !parent_a->node() ) {
                parent_a->node(node::create(parent));
            }
            parent_a->node()->add_child(gcomponent<actor>{go}->node());
        }
        return go;
    }

    gobject world::instantiate(const prefab& prefab, const node_iptr& parent) {
        gobject go = instantiate(prefab);
        if ( go && parent ) {
            parent->add_child(gcomponent<actor>{go}->node());
        }
        return go;
    }

    gobject world::instantiate(const gobject& parent, const t3f& transform) {
        gobject go = instantiate(parent);
        if ( go ) {
            gcomponent<actor>{go}->node()->transform(transform);
        }
        return go;
    }

    gobject world::instantiate(const node_iptr& parent, const t3f& transform) {
        gobject go = instantiate(parent);
        if ( go ) {
            gcomponent<actor>{go}->node()->transform(transform);
        }
        return go;
    }

    gobject world::instantiate(const prefab& prefab, const gobject& parent, const t3f& transform) {
        gobject go = instantiate(prefab, parent);
        if ( go ) {
            gcomponent<actor>{go}->node()->transform(transform);
        }
        return go;
    }

    gobject world::instantiate(const prefab& prefab, const node_iptr& parent, const t3f& transform) {
        gobject go = instantiate(prefab, parent);
        if ( go ) {
            gcomponent<actor>{go}->node()->transform(transform);
        }
        return go;
    }

    void world::destroy_instance(const gobject& inst) noexcept {
        gcomponent<actor> inst_a{inst};
        auto inst_n = inst_a ? inst_a->node() : nullptr;

        if ( inst_n ) {
            inst_n->for_each_child([this](const const_node_iptr& child_n){
                destroy_instance(child_n->owner());
            });
        }

        if ( inst ) {
            gobjects_.erase(inst.raw_entity().id());
            dynamic_pointer_cast<gobject_state>(inst.internal_state())->destroy();
        }
    }

    gobject world::resolve(ecs::entity_id ent) const noexcept {
        E2D_ASSERT(registry_.valid_entity(ent));
        const auto iter = gobjects_.find(ent);
        return iter != gobjects_.end()
            ? iter->second
            : gobject();
    }

    gobject world::resolve(const ecs::const_entity& ent) const noexcept {
        E2D_ASSERT(registry_.valid_entity(ent));
        const auto iter = gobjects_.find(ent.id());
        return iter != gobjects_.end()
            ? iter->second
            : gobject();
    }
}
