/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/strings.hpp>

#include <3rdparty/utfcpp/utf8.h>

namespace
{
    using namespace e2d;

    //
    // utf8_to_X
    //

    str16 utf8_to_16(str_view src) {
        str16 dst;
        dst.reserve(src.size());
        utf8::utf8to16(src.cbegin(), src.cend(), std::back_inserter(dst));
        dst.shrink_to_fit();
        return dst;
    }

    str32 utf8_to_32(str_view src) {
        str32 dst;
        dst.reserve(src.size());
        utf8::utf8to32(src.cbegin(), src.cend(), std::back_inserter(dst));
        dst.shrink_to_fit();
        return dst;
    }

    template < typename WChar = wchar_t >
    std::enable_if_t<sizeof(WChar) == 2, wstr>
    utf8_to_wide(str_view src) {
        wstr dst;
        dst.reserve(src.size());
        utf8::utf8to16(src.cbegin(), src.cend(), std::back_inserter(dst));
        dst.shrink_to_fit();
        return dst;
    }

    template < typename WChar = wchar_t >
    std::enable_if_t<sizeof(WChar) == 4, wstr>
    utf8_to_wide(str_view src) {
        wstr dst;
        dst.reserve(src.size());
        utf8::utf8to32(src.cbegin(), src.cend(), std::back_inserter(dst));
        dst.shrink_to_fit();
        return dst;
    }

    //
    // utf16_to_X
    //

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 2, str>
    utf16_to_8(basic_string_view<Char> src) {
        str dst;
        dst.reserve(src.size() * 2);
        utf8::utf16to8(src.cbegin(), src.cend(), std::back_inserter(dst));
        dst.shrink_to_fit();
        return dst;
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 2, str32>
    utf16_to_32(basic_string_view<Char> src) {
        return utf8_to_32(utf16_to_8(src));
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 2, wstr>
    utf16_to_wide(basic_string_view<Char> src) {
        return utf8_to_wide(utf16_to_8(src));
    }

    //
    // utf32_to_X
    //

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 4, str>
    utf32_to_8(basic_string_view<Char> src) {
        str dst;
        dst.reserve(src.size() * 4);
        utf8::utf32to8(src.cbegin(), src.cend(), std::back_inserter(dst));
        dst.shrink_to_fit();
        return dst;
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 4, str16>
    utf32_to_16(basic_string_view<Char> src) {
        return utf8_to_16(utf32_to_8(src));
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 4, wstr>
    utf32_to_wide(basic_string_view<Char> src) {
        return utf8_to_wide(utf32_to_8(src));
    }

    //
    // wide_to_X
    //

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 2, str>
    wide_to_utf8(basic_string_view<Char> src) {
        return utf16_to_8(src);
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 4, str>
    wide_to_utf8(basic_string_view<Char> src) {
        return utf32_to_8(src);
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 2, str16>
    wide_to_utf16(basic_string_view<Char> src) {
        return src.empty()
            ? str16()
            : str16(src.cbegin(), src.cend());
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 4, str16>
    wide_to_utf16(basic_string_view<Char> src) {
        return utf32_to_16(src);
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 2, str32>
    wide_to_utf32(basic_string_view<Char> src) {
        return utf16_to_32(src);
    }

    template < typename Char >
    std::enable_if_t<sizeof(Char) == 4, str32>
    wide_to_utf32(basic_string_view<Char> src) {
        return src.empty()
            ? str32()
            : str32(src.cbegin(), src.cend());
    }
}

namespace e2d
{
    //
    // make_utf8
    //

    str make_utf8(str_view src) {
        return src.empty()
            ? str()
            : str(src.cbegin(), src.cend());
    }

    str make_utf8(wstr_view src) {
        return wide_to_utf8(src);
    }

    str make_utf8(str16_view src) {
        return utf16_to_8(src);
    }

    str make_utf8(str32_view src) {
        return utf32_to_8(src);
    }

    //
    // make_wide
    //

    wstr make_wide(str_view src) {
        return utf8_to_wide(src);
    }

    wstr make_wide(wstr_view src) {
        return src.empty()
            ? wstr()
            : wstr(src.cbegin(), src.cend());
    }

    wstr make_wide(str16_view src) {
        return utf16_to_wide(src);
    }

    wstr make_wide(str32_view src) {
        return utf32_to_wide(src);
    }

    //
    // make_utf16
    //

    str16 make_utf16(str_view src) {
        return utf8_to_16(src);
    }

    str16 make_utf16(wstr_view src) {
        return wide_to_utf16(src);
    }

    str16 make_utf16(str16_view src) {
        return src.empty()
            ? str16()
            : str16(src.cbegin(), src.cend());
    }

    str16 make_utf16(str32_view src) {
        return utf32_to_16(src);
    }

    //
    // make_utf32
    //

    str32 make_utf32(str_view src) {
        return utf8_to_32(src);
    }

    str32 make_utf32(wstr_view src) {
        return wide_to_utf32(src);
    }

    str32 make_utf32(str16_view src) {
        return utf16_to_32(src);
    }

    str32 make_utf32(str32_view src) {
        return src.empty()
            ? str32()
            : str32(src.cbegin(), src.cend());
    }

    //
    // make_hash
    //

    str_hash make_hash(str_view src) noexcept {
        return str_hash(src);
    }

    wstr_hash make_hash(wstr_view src) noexcept {
        return wstr_hash(src);
    }

    str16_hash make_hash(str16_view src) noexcept {
        return str16_hash(src);
    }

    str32_hash make_hash(str32_view src) noexcept {
        return str32_hash(src);
    }
}

namespace e2d::strings
{
    namespace impl
    {
        // Inspired by:
        // https://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing

        using utf8_iter = utf8::iterator<str_view::const_iterator>;

        static bool wildcard_match_impl(
            utf8_iter string_i, utf8_iter string_e,
            utf8_iter pattern_i, utf8_iter pattern_e)
        {
            while ( pattern_i != pattern_e && *pattern_i != '*' ) {
                if ( string_i == string_e ) {
                    break;
                }
                if ( *pattern_i != *string_i && *pattern_i != '?' ) {
                    return false;
                }
                ++string_i;
                ++pattern_i;
            }
            if ( pattern_i == pattern_e ) {
                return string_i == string_e;
            }
            utf8_iter s_mark = string_e;
            utf8_iter p_mark = pattern_e;
            while ( string_i != string_e ) {
                if ( pattern_i != pattern_e ) {
                    if ( *pattern_i == '*' ) {
                        if ( ++pattern_i == pattern_e ) {
                            return true;
                        }
                        s_mark = string_i;
                        p_mark = pattern_i;
                        ++s_mark;
                        continue;
                    } else if ( *pattern_i == *string_i || *pattern_i == '?' ) {
                        ++string_i;
                        ++pattern_i;
                        continue;
                    }
                }
                string_i = s_mark;
                pattern_i = p_mark;
                if ( s_mark != string_e ) {
                    ++s_mark;
                }
            }
            while ( pattern_i != pattern_e && *pattern_i == '*' ) {
                ++pattern_i;
            }
            return pattern_i == pattern_e;
        }
    }

    bool wildcard_match(str_view string, str_view pattern) {
        using namespace impl;
        str_view::const_iterator si = string.cbegin();
        str_view::const_iterator se = string.cend();
        str_view::const_iterator pi = pattern.cbegin();
        str_view::const_iterator pe = pattern.cend();
        return wildcard_match_impl(
            utf8_iter(si, si, se), utf8_iter(se, si, se),
            utf8_iter(pi, pi, pe), utf8_iter(pe, pi, pe));
    }

    bool starts_with(str_view input, str_view test) noexcept {
        return input.size() >= test.size()
            && 0 == input.compare(0, test.size(), test);
    }

    bool ends_with(str_view input, str_view test) noexcept {
        return input.size() >= test.size()
            && 0 == input.compare(input.size() - test.size(), test.size(), test);
    }
}
