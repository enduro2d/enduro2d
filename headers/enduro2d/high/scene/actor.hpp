/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "node.hpp"

namespace e2d
{
    class actor;
    using actor_iptr = intrusive_ptr<actor>;
    using const_actor_iptr = intrusive_ptr<const actor>;
}

namespace e2d
{
    class actor final : public node {
    public:
        ~actor() noexcept final;
        static actor_iptr create(const ecs::entity& entity);

        ecs::entity entity() noexcept;
        ecs::const_entity entity() const noexcept;
    private:
        actor(const ecs::entity& entity);
    private:
        ecs::entity entity_;
    };
}
