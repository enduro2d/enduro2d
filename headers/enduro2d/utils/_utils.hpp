/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../base/_all.hpp"
#include "../math/_all.hpp"

namespace e2d
{
    class buffer;
    class color;
    class color32;
    class image;
    class jobber;
    class input_stream;
    class output_stream;
    class url;
}

namespace e2d
{
    using str   = basic_string<char>;
    using wstr  = basic_string<wchar_t>;
    using str16 = basic_string<char16_t>;
    using str32 = basic_string<char32_t>;

    using str_view   = basic_string_view<char>;
    using wstr_view  = basic_string_view<wchar_t>;
    using str16_view = basic_string_view<char16_t>;
    using str32_view = basic_string_view<char32_t>;

    struct seconds_tag {};
    struct milliseconds_tag {};
    struct microseconds_tag {};

    template < typename T >
    using seconds = unit<T, seconds_tag>;
    template < typename T >
    using milliseconds = unit<T, milliseconds_tag>;
    template < typename T >
    using microseconds = unit<T, microseconds_tag>;
}

namespace e2d
{
    class noncopyable {
    protected:
        noncopyable() = default;
        ~noncopyable() = default;
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}
