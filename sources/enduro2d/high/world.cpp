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
    private:
        enum flag_masks : u32 {
            fm_invalided = 1u << 0,
            fm_destroying = 1u << 1,
        };
    public:
        gobject_state(world& w, ecs::entity e)
        : world_(w)
        , entity_(std::move(e)) {}

        void mark_invalided() noexcept {
            math::set_flags_inplace(flags_, fm_invalided);
        }

        void mark_destroying() noexcept {
            math::set_flags_inplace(flags_, fm_destroying);
        }

        bool destroying() const noexcept {
            return math::check_any_flags(flags_, fm_destroying);
        }
    public:
        void destroy() noexcept final {
            gobject go{this};
            world_.destroy_instance(go);
        }

        bool invalided() const noexcept final {
            return math::check_any_flags(flags_, fm_invalided);
        }

        ecs::entity raw_entity() noexcept final {
            E2D_ASSERT(!invalided());
            return entity_;
        }

        ecs::const_entity raw_entity() const noexcept final {
            E2D_ASSERT(!invalided());
            return entity_;
        }
    private:
        world& world_;
        ecs::entity entity_;
        u32 flags_{0u};
    };
}

namespace e2d
{
    world::~world() noexcept {
        destroy_instances_();
        finalize_instances();
    }

    ecs::registry& world::registry() noexcept {
        return registry_;
    }

    const ecs::registry& world::registry() const noexcept {
        return registry_;
    }

    gobject world::instantiate() {
        gobject inst{make_intrusive<gobject_state>(
            *this,
            registry_.create_entity())};
        gobjects_.emplace(inst.raw_entity().id(), inst);

        try {
            auto n = node::create(inst);
            gcomponent<actor> inst_a{inst};
            if ( inst_a && inst_a->node() ) {
                n->transform(inst_a->node()->transform());
            }
            inst_a.assign(n);
        } catch (...) {
            finalize_instance_(inst);
            throw;
        }

        return inst;
    }

    gobject world::instantiate(const prefab& prefab) {
        gobject inst{make_intrusive<gobject_state>(
            *this,
            registry_.create_entity(prefab.prototype()))};
        gobjects_.emplace(inst.raw_entity().id(), inst);

        try {
            auto n = node::create(inst);
            gcomponent<actor> inst_a{inst};
            if ( inst_a && inst_a->node() ) {
                n->transform(inst_a->node()->transform());
            }
            inst_a.assign(n);
        } catch (...) {
            finalize_instance_(inst);
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
                    finalize_instance_(child);
                    throw;
                }
            }
        } catch (...) {
            finalize_instance_(inst);
            throw;
        }

        return inst;
    }

    gobject world::instantiate(const gobject& parent) {
        gobject go = instantiate();
        if ( go && parent ) {
            try {
                gcomponent<actor> parent_a{parent};
                if ( !parent_a ) {
                    parent_a.assign(node::create(parent));
                }
                if ( !parent_a->node() ) {
                    parent_a->node(node::create(parent));
                }
                parent_a->node()->add_child(gcomponent<actor>{go}->node());
            } catch (...) {
                finalize_instance_(go);
                throw;
            }
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
            try {
                gcomponent<actor> parent_a{parent};
                if ( !parent_a ) {
                    parent_a.assign(node::create(parent));
                }
                if ( !parent_a->node() ) {
                    parent_a->node(node::create(parent));
                }
                parent_a->node()->add_child(gcomponent<actor>{go}->node());
            } catch (...) {
                finalize_instance_(go);
                throw;
            }
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

    void world::destroy_instance(gobject& inst) noexcept {
        auto gstate = inst
            ? dynamic_pointer_cast<gobject_state>(inst.internal_state())
            : nullptr;
        if ( gstate && !gstate->destroying() ) {
            gstate->mark_destroying();
            destroying_states_.push_back(*gstate);
        }
    }

    void world::finalize_instances() noexcept {
        while ( !destroying_states_.empty() ) {
            gobject inst{&destroying_states_.front()};
            destroying_states_.pop_front();
            finalize_instance_(inst);
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

    void world::destroy_instances_() noexcept {
        for ( auto& [_, go] : gobjects_ ) {
            destroy_instance(go);
        }
    }

    void world::finalize_instance_(gobject& inst) noexcept {
        gcomponent<actor> inst_a{inst};
        auto inst_n = inst_a ? inst_a->node() : nullptr;

        if ( inst_n ) {
            inst_n->for_each_child([this](const node_iptr& child_n){
                destroy_instance(child_n->owner());
            });
        }

        if ( inst ) {
            inst.raw_entity().destroy();
            gobjects_.erase(inst.raw_entity().id());
            dynamic_pointer_cast<gobject_state>(inst.internal_state())->mark_invalided();
        }
    }
}
