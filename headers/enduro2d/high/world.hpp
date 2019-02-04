/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"
#include "scene/scene.hpp"

namespace e2d
{
    class world final : public module<world> {
    public:
        world();
        ~world() noexcept final;

        scene_iptr scene() noexcept;
        const_scene_iptr scene() const noexcept;

        ecs::registry& registry() noexcept;
        const ecs::registry& registry() const noexcept;
    private:
        scene_iptr scene_ = scene::create();
        ecs::registry registry_;
    };
}
