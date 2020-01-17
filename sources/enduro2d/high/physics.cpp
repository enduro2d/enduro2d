/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/physics.hpp>

namespace e2d
{
    physics::physics(starter::physics_parameters params)
    : params_(std::move(params)) {}

    physics::~physics() noexcept = default;

    const v2f& physics::gravity() const noexcept {
        return params_.gravity();
    }

    u32 physics::update_framerate() const noexcept {
        return params_.update_framerate();
    }

    u32 physics::velocity_iterations() const noexcept {
        return params_.velocity_iterations();
    }

    u32 physics::position_iterations() const noexcept {
        return params_.position_iterations();
    }
}
