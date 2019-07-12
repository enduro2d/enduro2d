/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    template < typename Char >
    class basic_string_hash final {
    public:
        basic_string_hash() noexcept = default;
        ~basic_string_hash() noexcept = default;

        basic_string_hash(basic_string_hash&& other) noexcept;
        basic_string_hash& operator=(basic_string_hash&& other) noexcept;

        basic_string_hash(const basic_string_hash& other) noexcept;
        basic_string_hash& operator=(const basic_string_hash& other) noexcept;

        basic_string_hash(const Char* str) noexcept;
        basic_string_hash(basic_string_view<Char> str) noexcept;

        basic_string_hash& assign(basic_string_hash&& other) noexcept;
        basic_string_hash& assign(const basic_string_hash& other) noexcept;
        basic_string_hash& assign(const Char* str) noexcept;
        basic_string_hash& assign(basic_string_view<Char> str) noexcept;

        void swap(basic_string_hash& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        u32 hash() const noexcept;
    private:
        static u32 empty_hash() noexcept;
        static u32 calculate_hash(basic_string_view<Char> str) noexcept;
        static void debug_check_collisions(u32 hash, basic_string_view<Char> str) noexcept;
    private:
        u32 hash_ = empty_hash();
    };

    template < typename Char >
    void swap(basic_string_hash<Char>& l, basic_string_hash<Char>& r) noexcept;

    template < typename Char >
    bool operator<(basic_string_hash<Char> l, basic_string_hash<Char> r) noexcept;

    template < typename Char >
    bool operator==(basic_string_hash<Char> l, basic_string_hash<Char> r) noexcept;

    template < typename Char >
    bool operator!=(basic_string_hash<Char> l, basic_string_hash<Char> r) noexcept;
}

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

    str_hash make_hash(str_view src) noexcept;
    wstr_hash make_hash(wstr_view src) noexcept;
    str16_hash make_hash(str16_view src) noexcept;
    str32_hash make_hash(str32_view src) noexcept;
}

namespace e2d::strings
{
    template < typename T >
    std::enable_if_t<
        std::is_integral_v<T> && std::is_signed_v<T>,
        bool>
    try_parse(str_view src, T& dst) noexcept;

    template < typename T >
    std::enable_if_t<
        std::is_integral_v<T> && std::is_unsigned_v<T>,
        bool>
    try_parse(str_view src, T& dst) noexcept;

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        bool>
    try_parse(str_view src, T& dst) noexcept;
}

namespace e2d::strings
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

    bool starts_with(str_view input, str_view test) noexcept;
    bool ends_with(str_view input, str_view test) noexcept;
}

#include "strings.inl"
