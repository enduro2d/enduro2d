/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/shape.hpp>
#include <enduro2d/utils/buffer.hpp>

namespace e2d { namespace shapes { namespace impl
{
    bool try_load_shape_e2d(shape& dst, const buffer& src) noexcept;
}}}
