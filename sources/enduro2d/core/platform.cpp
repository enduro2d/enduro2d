/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/platform.hpp>

namespace e2d
{
    //
    // platform::internal_state
    //

    class platform::internal_state final : private e2d::noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;
    };

    //
    // platform
    //

    platform::platform()
    : state_(new internal_state()) {}
    platform::~platform() noexcept = default;
}
