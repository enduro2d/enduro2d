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
        class transform2d {
        public:
            transform2d() = default;

            transform2d& position(const v2f& value) noexcept;
            transform2d& rotation(f32 value) noexcept;
            transform2d& scale(const v2f& value) noexcept;

            const v2f& position() const noexcept;
            f32 rotation() const noexcept;
            const v2f& scale() const noexcept;

            const m4f& as_matrix() const noexcept;
        private:
            v2f position_ = v2f::zero();
            f32 rotation_ = 0.f;
            v2f scale_ = v2f::unit();
        private:
            mutable m4f matrix_ = m4f::identity();
            mutable bool dirty_matrix_ = true;
        };

        class transform3d {
        public:
            transform3d() = default;

            transform3d& position(const v3f& value) noexcept;
            transform3d& rotation(const q4f& value) noexcept;
            transform3d& scale(const v3f& value) noexcept;

            const v3f& position() const noexcept;
            const q4f& rotation() const noexcept;
            const v3f& scale() const noexcept;

            const m4f& as_matrix() const noexcept;
        private:
            v3f position_ = v3f::zero();
            q4f rotation_ = q4f::identity();
            v3f scale_ = v3f::unit();
        private:
            mutable m4f matrix_ = m4f::identity();
            mutable bool dirty_matrix_ = true;
        };
    }
}
