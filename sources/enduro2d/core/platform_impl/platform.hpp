/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/core/platform.hpp>

namespace e2d
{
    class platform_internal_state_impl;
    using platform_internal_state_impl_uptr = std::unique_ptr<platform_internal_state_impl>;

    class platform_internal_state_impl : private e2d::noncopyable {
    public:
        static platform_internal_state_impl_uptr create();
    public:
        platform_internal_state_impl() = default;
        virtual ~platform_internal_state_impl() noexcept = default;
    };
}
