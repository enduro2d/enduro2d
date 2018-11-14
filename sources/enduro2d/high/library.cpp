/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/library.hpp>

namespace e2d
{
    //
    // library::internal_state
    //

    class library::internal_state final : private e2d::noncopyable {
    };

    //
    // library
    //

    library::library()
    : state_(new internal_state()){}
    library::~library() noexcept = default;
}
