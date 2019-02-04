/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/components/camera.hpp>

namespace e2d
{
    camera& camera::viewport(const b2u& value) noexcept {
        viewport_ = value;
        return *this;
    }

    camera& camera::projection(const m4f& value) noexcept {
        projection_ = value;
        return *this;
    }

    camera& camera::target(const render_target_ptr& value) noexcept {
        target_ = value;
        return *this;
    }

    camera& camera::background(const color& value) noexcept {
        background_ = value;
        return *this;
    }

    const b2u& camera::viewport() const noexcept {
        return viewport_;
    }

    const m4f& camera::projection() const noexcept {
        return projection_;
    }

    const render_target_ptr& camera::target() const noexcept {
        return target_;
    }

    const color& camera::background() const noexcept {
        return background_;
    }
}
