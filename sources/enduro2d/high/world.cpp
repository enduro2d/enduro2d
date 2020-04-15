/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
            inst_n->remove_from_parent();
            while ( const node_iptr& child = inst_n->first_child() ) {
                delete_instance(child->owner());
            }
        }

        if ( inst ) {
            auto inst_g = dynamic_pointer_cast<gobject_state>(inst.internal_state());
            inst_g->raw_entity().destroy();
            inst_g->mark_destroyed();
            inst_g->mark_invalided();
        }
    }

    gobject new_instance(world& world, const prefab& root_prefab) {
        ecs::entity ent = world.registry().create_entity(root_prefab.prototype());
        auto ent_defer = make_error_defer([&ent](){
            ent.destroy();
        });

        gobject root_i(make_intrusive<gobject_state>(world, ent));
        E2D_ERROR_DEFER([&root_i](){
            delete_instance(root_i);
        });

        ent_defer.dismiss();

        {
            gcomponent<actor> root_a{root_i};
            node_iptr new_root_node = node::create(root_i);
            if ( root_a && root_a->node() ) {
                new_root_node->transform(root_a->node()->transform());
            }
            root_a.ensure().node(new_root_node);
        }

        {
            gcomponent<behaviour> root_b{root_i};
            if ( root_b && root_b->script() ) {
                const behaviours::fill_result r = behaviours::fill_meta_table(*root_b);
                if ( r == behaviours::fill_result::failed ) {
                    root_i.component<disabled<behaviour>>().ensure();
                }
            }
        }

        for ( const prefab& child_prefab : root_prefab.children() ) {
            gobject child_i = new_instance(world, child_prefab);
            E2D_ERROR_DEFER([&child_i](){
                delete_instance(child_i);
            });
            gcomponent<actor> root_a{root_i};
            gcomponent<actor> child_a{child_i};
            root_a->node()->add_child(child_a->node());
        }

        return root_i;
    }

    void shutdown_instance(gobject& inst) noexcept {
        gcomponent<actor> inst_a = inst.component<actor>();
        if ( !inst_a ) {
            return;
        }
        nodes::for_extracted_components_from_children<behaviour>(
            inst_a->node(),
            [](gcomponent<behaviour>& inst_b){
                behaviours::call_meta_method(
                    *inst_b,
                    "on_shutdown",
                    inst_b.owner());
            }, nodes::options().recursive(true).include_root(true));
    }

    void start_instance(gobject& inst) {
        gcomponent<actor> inst_a = inst.component<actor>();
        if ( !inst_a ) {
            return;
        }
        nodes::for_extracted_components_from_children<behaviour>(
            inst_a->node(),
            [&inst](gcomponent<behaviour>& inst_b){
                const auto result = behaviours::call_meta_method(
                    *inst_b,
                    "on_start",
                    inst_b.owner());
                if ( result == behaviours::call_result::failed ) {
                    inst.component<disabled<behaviour>>().assign();
                }
            }, nodes::options().recursive(true).include_root(true));
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
        return instantiate(prefab(), nullptr);
    }

    gobject world::instantiate(const t2f& transform) {
        return instantiate(prefab(), nullptr, transform);
    }

    gobject world::instantiate(const node_iptr& parent) {
        return instantiate(prefab(), parent);
    }

    gobject world::instantiate(const node_iptr& parent, const t2f& transform) {
        return instantiate(prefab(), parent, transform);
    }

    gobject world::instantiate(const prefab& prefab) {
        return instantiate(prefab, nullptr);
    }

    gobject world::instantiate(const prefab& prefab, const t2f& transform) {
        return instantiate(prefab, nullptr, transform);
    }

    gobject world::instantiate(const prefab& prefab, const node_iptr& parent) {
        E2D_PROFILER_SCOPE("world.instantiate");

        gobject inst = new_instance(*this, prefab);
        E2D_ERROR_DEFER([inst](){
            delete_instance(inst);
        });

        if ( const node_iptr& node = inst.component<actor>()->node() ) {
            if ( parent ) {
                parent->add_child(node);
            }
        }

        start_instance(inst);
        return inst;
    }

    gobject world::instantiate(const prefab& prefab, const node_iptr& parent, const t2f& transform) {
        E2D_PROFILER_SCOPE("world.instantiate");

        gobject inst = new_instance(*this, prefab);
        E2D_ERROR_DEFER([inst](){
            delete_instance(inst);
        });

        if ( const node_iptr& node = inst.component<actor>()->node() ) {
            node->transform(transform);

            if ( parent ) {
                parent->add_child(node);
            }
        }

        start_instance(inst);
        return inst;
    }

    void world::destroy_instance(gobject inst) noexcept {
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
