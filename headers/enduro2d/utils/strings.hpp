/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_4540A3A22AD942D8A6B0C77A3346E73A
#define E2D_INCLUDE_GUARD_4540A3A22AD942D8A6B0C77A3346E73A
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
            str_view fmt, Args&&... args);

        template < typename... Args >
        bool format_nothrow(
            char* dst, std::size_t dst_size, std::size_t* length,
            str_view fmt, Args&&... args) noexcept;

        template < typename... Args >
        str rformat(
            str_view fmt, Args&&... args);

        template < typename... Args >
        bool rformat_nothrow(
            str& dst,
            str_view fmt, Args&&... args) noexcept;

        bool wildcard_match(str_view string, str_view pattern);
    }
}

#include "strings.inl"
#endif
