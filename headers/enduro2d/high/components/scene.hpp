/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "../node.hpp"

namespace e2d
{
    class scene final {
    public:
        scene() = default;

        scene& depth(i32 value) noexcept;
        i32 depth() const noexcept;
    private:
        i32 depth_ = 0;
    };
}

namespace e2d
{
    inline scene& scene::depth(i32 value) noexcept {
        depth_ = value;
        return *this;
    }

    inline i32 scene::depth() const noexcept {
        return depth_;
    }
}
