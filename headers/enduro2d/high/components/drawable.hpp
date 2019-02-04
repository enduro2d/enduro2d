/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "../scene.hpp"

namespace e2d
{
    namespace components
    {
        class drawable {
        public:
            drawable() = default;

            node_iptr node() noexcept;
            const_node_iptr node() const noexcept;
        private:
            node_iptr node_ = node::create();
        };
    }
}
