/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "node.hpp"
#include "prefab.hpp"
#include "gobject.hpp"

namespace e2d
{
    class world final : public module<world> {
    public:
        enum priorities : ecs::priority_t {
            priority_update_section_begin = 0,
                priority_pre_update = 500,
                priority_update = 1000,
                priority_post_update = 1500,
            priority_update_section_end = 2000,

            priority_render_section_begin = 2500,
                priority_pre_render = 3000,
                priority_render = 3500,
                priority_post_render = 4000,
            priority_render_section_end = 4500
        };
    public:
        world() = default;
        ~world() noexcept final;

        ecs::registry& registry() noexcept;
        const ecs::registry& registry() const noexcept;

        gobject instantiate();
        gobject instantiate(const prefab& prefab);

        gobject instantiate(const gobject& parent);
        gobject instantiate(const node_iptr& parent);

        gobject instantiate(const prefab& prefab, const gobject& parent);
        gobject instantiate(const prefab& prefab, const node_iptr& parent);

        gobject instantiate(const gobject& parent, const t3f& transform);
        gobject instantiate(const node_iptr& parent, const t3f& transform);

        gobject instantiate(const prefab& prefab, const gobject& parent, const t3f& transform);
        gobject instantiate(const prefab& prefab, const node_iptr& parent, const t3f& transform);

        void destroy_instance(const gobject& inst) noexcept;

        gobject resolve(ecs::entity_id ent) const noexcept;
        gobject resolve(const ecs::const_entity& ent) const noexcept;
    private:
        ecs::registry registry_;
        hash_map<ecs::entity_id, gobject> gobjects_;
    };
}
