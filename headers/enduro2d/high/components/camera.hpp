/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    class camera final {
    public:
        camera() = default;

        camera& viewport(const b2u& value) noexcept;
        camera& projection(const m4f& value) noexcept;
        camera& target(const render_target_ptr& value) noexcept;
        camera& background(const color& value) noexcept;

        const b2u& viewport() const noexcept;
        const m4f& projection() const noexcept;
        const render_target_ptr& target() const noexcept;
        const color& background() const noexcept;
    private:
        b2u viewport_ = b2u::zero();
        m4f projection_ = m4f::identity();
        render_target_ptr target_ = nullptr;
        color background_ = color::clear();
    };
}
