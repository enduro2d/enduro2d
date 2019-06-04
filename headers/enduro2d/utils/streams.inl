/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "streams.hpp"

namespace e2d
{
    //
    // input_sequence
    //

    template < typename T >
    std::enable_if_t<
        std::is_arithmetic<T>::value,
        input_sequence&>
    input_sequence::read(T& v) noexcept {
        return success_
            ? read(&v, sizeof(v))
            : *this;
    }

    //
    // output_sequence
    //

    template < typename T >
    std::enable_if_t<
        std::is_arithmetic<T>::value,
        output_sequence&>
    output_sequence::write(T v) noexcept {
        return success_
            ? write(&v, sizeof(v))
            : *this;
    }
}
