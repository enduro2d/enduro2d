/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/scene/actor.hpp>

namespace e2d
{
    actor::actor(const ecs::entity& entity)
    : entity_(entity) {}

    actor::~actor() noexcept = default;

    actor_iptr actor::create(const ecs::entity& entity) {
        return actor_iptr(new actor(entity));
    }

    ecs::entity actor::entity() noexcept {
        return entity_;
    }

    ecs::const_entity actor::entity() const noexcept {
        return entity_;
    }
}
