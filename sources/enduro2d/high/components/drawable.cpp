/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/components/drawable.hpp>

namespace e2d
{
    namespace components
    {
        node_iptr drawable::node() noexcept {
            return node_;
        }

        const_node_iptr drawable::node() const noexcept {
            return node_;
        }
    }
}
