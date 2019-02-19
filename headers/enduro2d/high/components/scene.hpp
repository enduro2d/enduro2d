/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "../node.hpp"

namespace e2d
{
    class scene final {
    public:
        scene() = default;

        scene& root(const node_iptr& value) noexcept;

        node_iptr root() noexcept;
        const_node_iptr root() const noexcept;
    private:
        node_iptr root_;
    };
}

namespace e2d
{
    inline scene& scene::root(const node_iptr& value) noexcept {
        root_ = value;
        return *this;
    }

    inline node_iptr scene::root() noexcept {
        return root_;
    }

    inline const_node_iptr scene::root() const noexcept {
        return root_;
    }
}
