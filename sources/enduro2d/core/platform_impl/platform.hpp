/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include <enduro2d/core/platform.hpp>

#define E2D_PLATFORM_MODE_NONE 1
#define E2D_PLATFORM_MODE_IOS 2
#define E2D_PLATFORM_MODE_LINUX 3
#define E2D_PLATFORM_MODE_MACOSX 4
#define E2D_PLATFORM_MODE_WINDOWS 5

#ifndef E2D_PLATFORM_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_PLATFORM_MODE E2D_PLATFORM_MODE_IOS
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_PLATFORM_MODE E2D_PLATFORM_MODE_LINUX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_PLATFORM_MODE E2D_PLATFORM_MODE_MACOSX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_PLATFORM_MODE E2D_PLATFORM_MODE_WINDOWS
#  endif
#endif

#ifndef E2D_PLATFORM_MODE
#  error E2D_PLATFORM_MODE not detected
#endif

namespace e2d
{
    class platform_internal_state_impl;
    using platform_internal_state_impl_uptr = std::unique_ptr<platform_internal_state_impl>;

    class platform_internal_state_impl : private e2d::noncopyable {
    public:
        platform_internal_state_impl() = default;
        ~platform_internal_state_impl() noexcept = default;
        static platform_internal_state_impl_uptr create();
    };
}
