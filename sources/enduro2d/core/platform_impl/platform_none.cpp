/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "platform.hpp"

#if defined(E2D_PLATFORM_MODE) && E2D_PLATFORM_MODE == E2D_PLATFORM_MODE_NONE

namespace
{
    using namespace e2d;

    class platform_internal_state_impl_none final : public platform_internal_state_impl {
    public:
        platform_internal_state_impl_none() = default;
        ~platform_internal_state_impl_none() noexcept final = default;
    };
}

namespace e2d
{
    platform_internal_state_impl_uptr platform_internal_state_impl::create() {
        return std::make_unique<platform_internal_state_impl_none>();
    }
}

#endif
