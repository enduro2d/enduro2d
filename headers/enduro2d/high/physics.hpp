/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "starter.hpp"

namespace e2d
{
    class physics final : public module<physics> {
    public:
        physics(starter::physics_parameters params);
        ~physics() noexcept final;

        const v2f& gravity() const noexcept;
        u32 update_framerate() const noexcept;
        u32 velocity_iterations() const noexcept;
        u32 position_iterations() const noexcept;
    private:
        starter::physics_parameters params_;
    };
}
