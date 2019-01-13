/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/components/transform.hpp>

namespace e2d
{
    namespace components
    {
        //
        // transform2d
        //

        transform2d& transform2d::position(const v2f& value) noexcept {
            position_ = value;
            dirty_matrix_ = true;
            return *this;
        }

        transform2d& transform2d::rotation(f32 value) noexcept {
            rotation_ = value;
            dirty_matrix_ = true;
            return *this;
        }

        transform2d& transform2d::scale(const v2f& value) noexcept {
            scale_ = value;
            dirty_matrix_ = true;
            return *this;
        }

        const v2f& transform2d::position() const noexcept {
            return position_;
        }

        f32 transform2d::rotation() const noexcept {
            return rotation_;
        }

        const v2f& transform2d::scale() const noexcept {
            return scale_;
        }

        const m4f& transform2d::as_matrix() const noexcept {
            if ( dirty_matrix_ ) {
                matrix_ = math::make_scale_matrix4(scale_)
                        * math::make_rotation_matrix4(make_rad(rotation_), v4f::unit_z())
                        * math::make_translation_matrix4(position_);
                dirty_matrix_ = false;
            }
            return matrix_;
        }

        //
        // transform3d
        //

        transform3d& transform3d::position(const v3f& value) noexcept {
            position_ = value;
            dirty_matrix_ = true;
            return *this;
        }

        transform3d& transform3d::rotation(const q4f& value) noexcept {
            rotation_ = value;
            dirty_matrix_ = true;
            return *this;
        }

        transform3d& transform3d::scale(const v3f& value) noexcept {
            scale_ = value;
            dirty_matrix_ = true;
            return *this;
        }

        const v3f& transform3d::position() const noexcept {
            return position_;
        }

        const q4f& transform3d::rotation() const noexcept {
            return rotation_;
        }

        const v3f& transform3d::scale() const noexcept {
            return scale_;
        }

        const m4f& transform3d::as_matrix() const noexcept {
            if ( dirty_matrix_ ) {
                matrix_ = math::make_scale_matrix4(scale_)
                        * math::make_rotation_matrix4(rotation_)
                        * math::make_translation_matrix4(position_);
                dirty_matrix_ = false;
            }
            return matrix_;
        }
    }
}
