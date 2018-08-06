/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    str make_utf8(str_view src);
    str make_utf8(wstr_view src);
    str make_utf8(str16_view src);
    str make_utf8(str32_view src);

    wstr make_wide(str_view src);
    wstr make_wide(wstr_view src);
    wstr make_wide(str16_view src);
    wstr make_wide(str32_view src);

    str16 make_utf16(str_view src);
    str16 make_utf16(wstr_view src);
    str16 make_utf16(str16_view src);
    str16 make_utf16(str32_view src);

    str32 make_utf32(str_view src);
    str32 make_utf32(wstr_view src);
    str32 make_utf32(str16_view src);
    str32 make_utf32(str32_view src);

    namespace strings
    {
        class format_error;
        class bad_format;
        class bad_format_buffer;
        class bad_format_argument;

        template < typename T, typename = void >
        class format_arg;

        template < typename T, typename... Args >
        format_arg<std::decay_t<T>> make_format_arg(T&& v, Args&&... args);

        template < typename... Args >
        std::size_t format(
            char* dst, std::size_t size,
            const char* fmt, Args&&... args);

        template < typename... Args >
        str rformat(const char* fmt, Args&&... args);

        bool wildcard_match(str_view string, str_view pattern);
    }
}

#include "strings.inl"
