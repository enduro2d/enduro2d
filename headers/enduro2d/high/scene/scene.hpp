/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "node.hpp"

namespace e2d
{
    class scene
        : private noncopyable
        , public ref_counter<scene> {
    public:
        virtual ~scene() noexcept;
        static scene_iptr create();

        node_iptr root() noexcept;
        const_node_iptr root() const noexcept;
    protected:
        scene();
    private:
        node_iptr root_ = node::create();
    };
}
