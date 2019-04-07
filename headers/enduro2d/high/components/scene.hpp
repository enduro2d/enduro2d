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
        scene(const node_iptr& root) noexcept;

        scene& depth(i32 value) noexcept;
        scene& root(const node_iptr& value) noexcept;

        i32 depth() const noexcept;
        node_iptr root() noexcept;
        const_node_iptr root() const noexcept;
    private:
        i32 depth_ = 0;
        node_iptr root_;
    };
}

namespace e2d
{
    inline scene::scene(const node_iptr& root) noexcept
    : root_(root) {}

    inline scene& scene::depth(i32 value) noexcept {
        depth_ = value;
        return *this;
    }

    inline scene& scene::root(const node_iptr& value) noexcept {
        root_ = value;
        return *this;
    }

    inline i32 scene::depth() const noexcept {
        return depth_;
    }

    inline node_iptr scene::root() noexcept {
        return root_;
    }

    inline const_node_iptr scene::root() const noexcept {
        return root_;
    }
}
