/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/world.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/disabled.hpp>

namespace
{
    using namespace e2d;

    class gobject_state final : public gobject::state {
    private:
        enum flag_masks : u32 {
            fm_destroyed = 1u << 0,
            fm_invalided = 1u << 1,
        };
    public:
        gobject_state(world& w, ecs::entity e)
        : world_(w)
        , entity_(std::move(e)) {}

        void mark_destroyed() noexcept {
            math::set_flags_inplace(flags_, fm_destroyed);
        }

        void mark_invalided() noexcept {
            math::set_flags_inplace(flags_, fm_invalided);
        }
    public:
        void destroy() noexcept final {
            gobject go{this};
            world_.destroy_instance(go);
        }

        bool destroyed() const noexcept final {
            return math::check_any_flags(flags_, fm_destroyed);
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

namespace
{
    using namespace e2d;

    void delete_instance(const gobject& inst) noexcept {
        gcomponent<actor> inst_a{inst};
        auto inst_n = inst_a ? inst_a->node() : nullptr;

        if ( inst_n ) {
            inst_n->for_each_child([](const node_iptr& child_n){
                delete_instance(child_n->owner());
            });
        }

        if ( inst ) {
            auto inst_g = dynamic_pointer_cast<gobject_state>(inst.internal_state());
            inst_g->raw_entity().destroy();
            inst_g->mark_destroyed();
            inst_g->mark_invalided();
        }
    }

    gobject new_instance(world& world, const prefab& prefab) {
        gobject inst;
        ecs::entity ent = world.registry().create_entity(prefab.prototype());

        try {
            inst = gobject{make_intrusive<gobject_state>(world, ent)};
        } catch (...) {
            ent.destroy();
            throw;
        }

        try {
            auto n = node::create(inst);
            gcomponent<actor> inst_a{inst};
            if ( inst_a && inst_a->node() ) {
                n->transform(inst_a->node()->transform());
            }
            inst_a.assign(std::move(n));
        } catch (...) {
            delete_instance(inst);
            throw;
        }

        try {
            gcomponent<behaviour> inst_b{inst};
            if ( inst_b && inst_b->script() ) {
                const behaviours::fill_result r = behaviours::fill_meta_table(*inst_b);
                if ( r == behaviours::fill_result::failed ) {
                    inst.component<disabled<behaviour>>().ensure();
                }
            }
        } catch (...) {
            delete_instance(inst);
            throw;
        }

        try {
            for ( const auto& child_prefab : prefab.children() ) {
                auto child = new_instance(world, child_prefab);
                try {
                    gcomponent<actor> inst_a{inst};
                    gcomponent<actor> child_a{child};
                    inst_a->node()->add_child(child_a->node());
                } catch (...) {
                    delete_instance(child);
                    throw;
                }
            }
        } catch (...) {
            delete_instance(inst);
            throw;
        }

        return inst;
    }

    void shutdown_instance(gobject& inst) noexcept {
        if ( gcomponent<actor> inst_a{inst}; inst_a ) {
            nodes::for_extracted_nodes(inst_a->node(), [](const node_iptr& node){
                if ( gcomponent<behaviour> inst_b{node->owner()}; inst_b ) {
                    behaviours::call_meta_method(
                        *inst_b,
                        "on_shutdown",
                        node->owner());
                }
            });
        }
    }

    void start_instance(gobject& inst) {
        if ( gcomponent<actor> inst_a{inst}; inst_a ) {
            nodes::for_extracted_nodes(inst_a->node(), [&inst](const node_iptr& node){
                if ( gcomponent<behaviour> inst_b{node->owner()}; inst_b ) {
                    const auto result = behaviours::call_meta_method(
                        *inst_b,
                        "on_start",
                        node->owner());
                    if ( result == behaviours::call_result::failed ) {
                        inst.component<disabled<behaviour>>().assign();
                    }
                }
            });
        }
    }
}

namespace e2d
{
    world::~world() noexcept = default;

    ecs::registry& world::registry() noexcept {
        return registry_;
    }

    const ecs::registry& world::registry() const noexcept {
        return registry_;
    }

    gobject world::instantiate() {
        return instantiate(nullptr);
    }

    gobject world::instantiate(const prefab& prefab) {
        return instantiate(prefab, nullptr);
    }

    gobject world::instantiate(const node_iptr& parent) {
        return instantiate(prefab(), parent);
    }

    gobject world::instantiate(const prefab& prefab, const node_iptr& parent) {
        E2D_PROFILER_SCOPE("world.instantiate");

        gobject inst = new_instance(*this, prefab);

        if ( parent ) {
            parent->add_child(inst.component<actor>()->node());
        }

        try {
            start_instance(inst);
        } catch (...) {
            delete_instance(inst);
            throw;
        }

        return inst;
    }

    gobject world::instantiate(const node_iptr& parent, const t2f& transform) {
        return instantiate(prefab(), parent, transform);
    }

    gobject world::instantiate(const prefab& prefab, const node_iptr& parent, const t2f& transform) {
        E2D_PROFILER_SCOPE("world.instantiate");

        gobject inst = new_instance(*this, prefab);
        inst.component<actor>()->node()->transform(transform);

        if ( parent ) {
            parent->add_child(inst.component<actor>()->node());
        }

        try {
            start_instance(inst);
        } catch (...) {
            delete_instance(inst);
            throw;
        }

        return inst;
    }

    void world::destroy_instance(gobject& inst) noexcept {
        auto gstate = inst
            ? dynamic_pointer_cast<gobject_state>(inst.internal_state())
            : nullptr;
        if ( gstate && !gstate->destroyed() && !gstate->invalided() ) {
            gstate->mark_destroyed();
            destroying_states_.push_back(*gstate);
        }
    }

    void world::finalize_instances() noexcept {
        E2D_PROFILER_SCOPE("world.finalize_instances");
        while ( !destroying_states_.empty() ) {
            gobject inst{&destroying_states_.front()};
            destroying_states_.pop_front();
            shutdown_instance(inst);
            delete_instance(inst);
        }
    }
}
