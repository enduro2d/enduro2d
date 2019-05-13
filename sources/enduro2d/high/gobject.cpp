/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/gobject.hpp>

namespace e2d
{
    gobject::gobject(ecs::registry& registry)
    : entity_(registry.create_entity()) {}

    gobject::gobject(ecs::registry& registry, const ecs::prototype& proto)
    : entity_(registry.create_entity(proto)) {}

    gobject::~gobject() noexcept {
        entity_.destroy();
    }

    ecs::entity gobject::entity() noexcept {
        return entity_;
    }

    ecs::const_entity gobject::entity() const noexcept {
        return entity_;
    }

    ecs::entity_filler gobject::entity_filler() noexcept {
        return ecs::entity_filler(entity_);
    }
}
