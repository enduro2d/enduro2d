/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    namespace components
    {
        class sprite {
        public:
            sprite() = default;

            sprite& tint(const color& value) noexcept;
            sprite& texture(const texture_ptr& value) noexcept;
            sprite& texture_rect(const b2f& value) noexcept;

            const color& tint() const noexcept;
            const texture_ptr& texture() const noexcept;
            const b2f& texture_rect() const noexcept;
        private:
            color tint_ = color::white();
            texture_ptr texture_ = nullptr;
            b2f texture_rect_ = b2f{0.f, 0.f, 1.f, 1.f};
        };
    }
}
