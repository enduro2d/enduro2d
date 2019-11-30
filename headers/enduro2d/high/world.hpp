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
        world() = default;
        ~world() noexcept final;

        ecs::registry& registry() noexcept;
        const ecs::registry& registry() const noexcept;

        gobject instantiate();
        gobject instantiate(const prefab& prefab);

        gobject instantiate(const node_iptr& parent);
        gobject instantiate(const prefab& prefab, const node_iptr& parent);

        gobject instantiate(const node_iptr& parent, const t2f& transform);
        gobject instantiate(const prefab& prefab, const node_iptr& parent, const t2f& transform);

        void destroy_instance(gobject& inst) noexcept;
        void finalize_instances() noexcept;
    private:
        ecs::registry registry_;
        gobject::destroying_states destroying_states_;
    };
}
