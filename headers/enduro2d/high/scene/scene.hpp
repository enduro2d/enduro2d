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
    class scene;
    using scene_iptr = intrusive_ptr<scene>;
    using const_scene_iptr = intrusive_ptr<const scene>;
}

namespace e2d
{
    class scene final
        : private noncopyable
        , public ref_counter<scene> {
    public:
        ~scene() noexcept;
        static scene_iptr create();

        const node_iptr& root() const noexcept;
    private:
        scene();
    private:
        node_iptr root_ = node::create();
    };
}
