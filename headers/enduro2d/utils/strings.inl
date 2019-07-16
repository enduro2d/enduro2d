/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "strings.hpp"

namespace e2d
{
    template < typename Char >
    basic_string_hash<Char>::basic_string_hash(
        basic_string_hash&& other) noexcept
    {
        assign(std::move(other));
    }

    template < typename Char >
    basic_string_hash<Char>& basic_string_hash<Char>::operator=(
        basic_string_hash&& other) noexcept
    {
        return assign(std::move(other));
    }

    template < typename Char >
    basic_string_hash<Char>::basic_string_hash(
        const basic_string_hash& other) noexcept
    {
        assign(other);
    }

    template < typename Char >
    basic_string_hash<Char>& basic_string_hash<Char>::operator=(
        const basic_string_hash& other) noexcept
    {
        return assign(other);
    }

    template < typename Char >
    basic_string_hash<Char>::basic_string_hash(
        const Char* str) noexcept
    {
        assign(str);
    }

    template < typename Char >
    basic_string_hash<Char>::basic_string_hash(
        basic_string_view<Char> str) noexcept
    {
        assign(str);
    }

    template < typename Char >
    basic_string_hash<Char>& basic_string_hash<Char>::assign(
        basic_string_hash&& other) noexcept
    {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    template < typename Char >
    basic_string_hash<Char>& basic_string_hash<Char>::assign(
        const basic_string_hash& other) noexcept
    {
        if ( this != &other ) {
            hash_ = other.hash_;
        }
        return *this;
    }

    template < typename Char >
    basic_string_hash<Char>& basic_string_hash<Char>::assign(
        const Char* str) noexcept
    {
        hash_ = calculate_hash(str);
        return *this;
    }

    template < typename Char >
    basic_string_hash<Char>& basic_string_hash<Char>::assign(
        basic_string_view<Char> str) noexcept
    {
        hash_ = calculate_hash(str);
        return *this;
    }

    template < typename Char >
    void basic_string_hash<Char>::swap(basic_string_hash& other) noexcept {
        using std::swap;
        swap(hash_, other.hash_);
    }

    template < typename Char >
    void basic_string_hash<Char>::clear() noexcept {
        hash_ = empty_hash();
    }

    template < typename Char >
    bool basic_string_hash<Char>::empty() const noexcept {
        return hash_ == empty_hash();
    }

    template < typename Char >
    u32 basic_string_hash<Char>::hash() const noexcept {
        return hash_;
    }

    template < typename Char >
    u32 basic_string_hash<Char>::empty_hash() noexcept {
        static u32 hash = calculate_hash(basic_string_view<Char>());
        return hash;
    }

    template < typename Char >
    u32 basic_string_hash<Char>::calculate_hash(basic_string_view<Char> str) noexcept {
        u32 hash = utils::sdbm_hash(str);
        debug_check_collisions(hash, str);
        return hash;
    }

    template < typename Char >
    void basic_string_hash<Char>::debug_check_collisions(u32 hash, basic_string_view<Char> str) noexcept {
    #if defined(E2D_BUILD_MODE) && E2D_BUILD_MODE == E2D_BUILD_MODE_DEBUG
        try {
            static std::mutex mutex;
            static hash_map<u32, basic_string<Char>> table;
            std::lock_guard<std::mutex> guard(mutex);
            const auto iter = table.find(hash);
            if ( iter != table.end() ) {
                E2D_ASSERT_MSG(
                    iter->second == str,
                    "basic_string_hash: hash collision detected");
            } else {
                table.insert(std::make_pair(hash, str));
            }
        } catch (...) {
            E2D_ASSERT_MSG(false, "basic_string_hash: unexpected debug exception");
        }
    #else
        E2D_UNUSED(hash, str);
    #endif
    }
}

namespace e2d
{
    template < typename Char >
    void swap(basic_string_hash<Char>& l, basic_string_hash<Char>& r) noexcept {
        l.swap(r);
    }

    template < typename Char >
    bool operator<(basic_string_hash<Char> l, basic_string_hash<Char> r) noexcept {
        return l.hash() < r.hash();
    }

    template < typename Char >
    bool operator==(basic_string_hash<Char> l, basic_string_hash<Char> r) noexcept {
        return l.hash() == r.hash();
    }

    template < typename Char >
    bool operator!=(basic_string_hash<Char> l, basic_string_hash<Char> r) noexcept {
        return !(l == r);
    }
}

namespace std
{
    template < typename Char >
    struct hash<e2d::basic_string_hash<Char>> {
        std::size_t operator()(e2d::basic_string_hash<Char> hs) const noexcept {
            return hs.hash();
        }
    };
}

namespace e2d::strings
{
    template < typename T >
    std::enable_if_t<
        std::is_integral_v<T> && std::is_signed_v<T>,
        bool>
    try_parse(str_view src, T& dst) noexcept {
        char str[32] = {'\0'};
        if ( src.size() >= std::size(str) ) {
            return false;
        }

        errno = 0;
        char* end = nullptr;
        std::memcpy(str, src.data(), src.size());
        const i64 tmp = std::strtoll(str, &end, 0);

        if ( str == end || end != str + src.size() || errno ) {
            return false;
        }

        if ( tmp > std::numeric_limits<T>::max() ) {
            return false;
        }

        if ( tmp < std::numeric_limits<T>::lowest() ) {
            return false;
        }

        dst = static_cast<T>(tmp);
        return true;
    }

    template < typename T >
    std::enable_if_t<
        std::is_integral_v<T> && std::is_unsigned_v<T>,
        bool>
    try_parse(str_view src, T& dst) noexcept {
        char str[32] = {'\0'};
        if ( src.size() >= std::size(str) ) {
            return false;
        }

        errno = 0;
        char* end = nullptr;
        std::memcpy(str, src.data(), src.size());
        const i64 tmp = std::strtoll(str, &end, 0);

        if ( str == end || end != str + src.size() || errno ) {
            return false;
        }

        if ( tmp < 0 ) {
            return false;
        }

        const u64 utmp = static_cast<u64>(tmp);

        if ( utmp > std::numeric_limits<T>::max() ) {
            return false;
        }

        dst = static_cast<T>(utmp);
        return true;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point_v<T>,
        bool>
    try_parse(str_view src, T& dst) noexcept {
        char str[128] = {'\0'};
        if ( src.size() >= std::size(str) ) {
            return false;
        }

        errno = 0;
        char* end = nullptr;
        std::memcpy(str, src.data(), src.size());
        const f32 tmp = std::strtof(str, &end);

        if ( str == end || end != str + src.size() || errno ) {
            return false;
        }

        if ( tmp > std::numeric_limits<T>::max() ) {
            return false;
        }

        if ( tmp < std::numeric_limits<T>::lowest() ) {
            return false;
        }

        dst = static_cast<T>(tmp);
        return true;
    }
}

namespace e2d::strings
{
    //
    // exceptions
    //

    class format_error : public exception {
    public:
        const char* what() const noexcept override {
            return "format error";
        }
    };

    class bad_format final : public format_error {
    public:
        const char* what() const noexcept final {
            return "bad format";
        }
    };

    class bad_format_buffer final : public format_error {
    public:
        const char* what() const noexcept final {
            return "bad format buffer";
        }
    };

    class bad_format_argument final : public format_error {
    public:
        const char* what() const noexcept final {
            return "bad format argument";
        }
    };

    //
    // arguments
    //

    template < typename T, typename... Args >
    format_arg<std::decay_t<T>> make_format_arg(T&& v, Args&&... args) {
        return format_arg<std::decay_t<T>>(
            std::forward<T>(v), std::forward<Args>(args)...);
    }

    namespace impl
    {
        // Inspired by:
        // https://github.com/miloyip/itoa-benchmark

        static constexpr char u8toa_lt[200] = {
            '0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
            '1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
            '2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
            '3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
            '4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
            '5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
            '6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
            '7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
            '8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
            '9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
        };

        inline char* u8toa(u8 value, char* buffer) noexcept {
            E2D_ASSERT(buffer);
            const i32 d1 = (value / 100) << 1;
            const i32 d2 = (value % 100) << 1;
            if ( value >= 100 ) {
                *buffer++ = u8toa_lt[d1 + 1];
            }
            if ( value >= 10 ) {
                *buffer++ = u8toa_lt[d2];
            }
            *buffer++ = u8toa_lt[d2 + 1];
            return buffer;
        }
    }

    template < typename T >
    class format_arg<T, std::enable_if_t<
        std::is_integral_v<T> && std::is_signed_v<T>>>
    {
        T value_;
        u8 width_;
    public:
        explicit format_arg(T value, u8 width = 0) noexcept
        : value_(value), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const noexcept {
            char format[7] = {0};
            char* b_format = format;
            *b_format++ = '%';
            b_format = impl::u8toa(width_, b_format);
            *b_format++ = 'j';
            *b_format++ = 'i';
            E2D_ASSERT(b_format < format + sizeof(format));
            return std::snprintf(
                dst, size, format,
                math::numeric_cast<std::intmax_t>(value_));
        }
    };

    template < typename T >
    class format_arg<T, std::enable_if_t<
        std::is_integral_v<T> && std::is_unsigned_v<T>>>
    {
        T value_;
        u8 width_;
    public:
        explicit format_arg(T value, u8 width = 0) noexcept
        : value_(value), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const noexcept {
            char format[7] = {0};
            char* b_format = format;
            *b_format++ = '%';
            b_format = impl::u8toa(width_, b_format);
            *b_format++ = 'j';
            *b_format++ = 'u';
            E2D_ASSERT(b_format < format + sizeof(format));
            return std::snprintf(
                dst, size, format,
                math::numeric_cast<std::uintmax_t>(value_));
        }
    };

    template < typename T >
    class format_arg<T, std::enable_if_t<
        std::is_floating_point_v<T>>>
    {
        T value_;
        u8 width_;
        u8 precision_;
    public:
        explicit format_arg(T value, u8 width = 0, u8 precision = 6) noexcept
        : value_(value), width_(width), precision_(precision) {}

        std::ptrdiff_t write(char* dst, size_t size) const noexcept {
            char format[11] = {0};
            char* b_format = format;
            *b_format++ = '%';
            b_format = impl::u8toa(width_, b_format);
            *b_format++ = '.';
            b_format = impl::u8toa(precision_, b_format);
            *b_format++ = 'L';
            *b_format++ = 'f';
            E2D_ASSERT(b_format < format + sizeof(format));
            return std::snprintf(
                dst, size, format,
                math::numeric_cast<long double>(value_));
        }
    };

    template <>
    class format_arg<bool> {
        bool value_;
    public:
        explicit format_arg(bool value) noexcept
        : value_(value) {}

        std::ptrdiff_t write(char* dst, size_t size) const noexcept {
            return std::snprintf(
                dst, size, "%s",
                value_ ? "true" : "false");
        }
    };

    template <>
    class format_arg<char*> {
        const char* value_;
        u8 width_;
    public:
        explicit format_arg(const char* value, u8 width = 0) noexcept
        : value_(value), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const noexcept {
            if ( !value_ ) {
                return 0;
            }
            char format[6] = {0};
            char* b_format = format;
            *b_format++ = '%';
            b_format = impl::u8toa(width_, b_format);
            *b_format++ = 's';
            E2D_ASSERT(b_format < format + sizeof(format));
            return std::snprintf(
                dst, size, format, value_);
        }
    };

    template <>
    class format_arg<const char*> {
        const char* value_;
        u8 width_;
    public:
        explicit format_arg(const char* value, u8 width = 0) noexcept
        : value_(value), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const noexcept {
            if ( !value_ ) {
                return 0;
            }
            char format[6] = {0};
            char* b_format = format;
            *b_format++ = '%';
            b_format = impl::u8toa(width_, b_format);
            *b_format++ = 's';
            E2D_ASSERT(b_format < format + sizeof(format));
            return std::snprintf(
                dst, size, format, value_);
        }
    };

    namespace impl
    {
        template < typename T >
        struct is_arg_impl
        : std::false_type {};

        template < typename T >
        struct is_arg_impl<format_arg<T>>
        : std::true_type {};

        template < typename T >
        struct is_arg
        : is_arg_impl<std::remove_cv_t<T>> {};

        template < typename T >
        inline constexpr bool is_arg_v = is_arg<T>::value;

        template < typename T >
        std::enable_if_t<is_arg_v<std::decay_t<T>>, T>
        wrap_arg(T&& arg) {
            return std::forward<T>(arg);
        }

        template < typename T >
        std::enable_if_t<!is_arg_v<std::decay_t<T>>, format_arg<std::decay_t<T>>>
        wrap_arg(T&& value) {
            return make_format_arg(std::forward<T>(value));
        }

        template < std::size_t N, typename Tuple >
        std::enable_if_t<N < std::tuple_size<Tuple>::value, std::ptrdiff_t>
        write_arg_n(char* dst, std::size_t size, const Tuple& targs) {
            return std::get<N>(targs).write(dst, size);
        }

        template < std::size_t N, typename Tuple >
        std::enable_if_t<N >= std::tuple_size<Tuple>::value, std::ptrdiff_t>
        write_arg_n(char* dst, std::size_t size, const Tuple& targs) {
            E2D_UNUSED(dst, size, targs);
            if ( dst ) {
                *dst = '\0';
            }
            throw bad_format();
        }

        template < typename Tuple >
        std::enable_if_t<std::tuple_size<Tuple>::value <= 10, std::size_t>
        format_impl(char* dst, std::size_t size, str_view fmt, const Tuple& targs) {
            str_view::const_iterator format_i = fmt.cbegin();
            const str_view::const_iterator format_e = fmt.cend();

            if ( !dst != !size ) {
                throw bad_format_buffer();
            }

            std::size_t result = 0;
            const char* const b_dst = dst;
            const char* const e_dst = b_dst ? b_dst + size : nullptr;

            while ( format_i != format_e ) {
                if ( *format_i != '%' ) {
                    if ( dst && dst == e_dst - 1 ) {
                        *dst = '\0';
                        throw bad_format_buffer();
                    }
                    if ( dst ) {
                        *dst++ = *format_i;
                    }
                    ++result;
                    ++format_i;
                } else {
                    if ( ++format_i == format_e ) {
                        // "hello%"
                        if ( dst ) {
                            *dst = '\0';
                        }
                        throw bad_format();
                    }
                    std::ptrdiff_t write_arg_r = 0;
                    std::size_t dst_tail_size = dst
                        ? math::numeric_cast<std::size_t>(e_dst - dst)
                        : 0;
                    E2D_ASSERT(!dst || dst_tail_size);
                    switch ( *format_i ) {
                        case '0' :
                            write_arg_r = write_arg_n<0>(dst, dst_tail_size, targs);
                            break;
                        case '1' :
                            write_arg_r = write_arg_n<1>(dst, dst_tail_size, targs);
                            break;
                        case '2' :
                            write_arg_r = write_arg_n<2>(dst, dst_tail_size, targs);
                            break;
                        case '3' :
                            write_arg_r = write_arg_n<3>(dst, dst_tail_size, targs);
                            break;
                        case '4' :
                            write_arg_r = write_arg_n<4>(dst, dst_tail_size, targs);
                            break;
                        case '5' :
                            write_arg_r = write_arg_n<5>(dst, dst_tail_size, targs);
                            break;
                        case '6' :
                            write_arg_r = write_arg_n<6>(dst, dst_tail_size, targs);
                            break;
                        case '7' :
                            write_arg_r = write_arg_n<7>(dst, dst_tail_size, targs);
                            break;
                        case '8' :
                            write_arg_r = write_arg_n<8>(dst, dst_tail_size, targs);
                            break;
                        case '9' :
                            write_arg_r = write_arg_n<9>(dst, dst_tail_size, targs);
                            break;
                        case '%':
                            // "hel%%lo" -> "hel%lo"
                            if ( dst ) {
                                *dst = '%';
                            }
                            write_arg_r = 1;
                            break;
                        default:
                            // "hel%xlo"
                            if ( dst ) {
                                *dst = '\0';
                            }
                            throw bad_format();
                    }
                    if ( write_arg_r < 0 ) {
                        if ( dst ) {
                            *dst = '\0';
                        }
                        throw bad_format_argument();
                    }
                    const std::size_t write_bytes =
                        math::abs_to_unsigned(write_arg_r);
                    if ( size && write_bytes >= dst_tail_size ) {
                        if ( dst ) {
                            E2D_ASSERT(b_dst + size == dst + dst_tail_size);
                            *(dst + dst_tail_size - 1) = '\0';
                        }
                        throw bad_format_buffer();
                    }
                    if ( dst ) {
                        dst += write_bytes;
                    }
                    result += write_bytes;
                    ++format_i;
                }
            }
            if ( dst ) {
                *dst = '\0';
            }
            E2D_ASSERT(
                !dst ||
                result == math::numeric_cast<std::size_t>(dst - b_dst));
            return result;
        }
    }

    template < typename... Args >
    std::size_t format(
        char* dst, std::size_t size,
        str_view fmt, Args&&... args)
    {
        return impl::format_impl(
            dst, size, fmt,
            std::make_tuple(impl::wrap_arg(std::forward<Args>(args))...));
    }

    template < typename... Args >
    bool format_nothrow(
        char* dst, std::size_t dst_size, std::size_t* length,
        str_view fmt, Args&&... args) noexcept
    {
        try {
            std::size_t result = format(
                dst, dst_size, fmt, std::forward<Args>(args)...);
            if ( length ) {
                *length = result;
            }
            return true;
        } catch (...) {
            return false;
        }
    }

    template < typename... Args >
    str rformat(str_view fmt, Args&&... args) {
        auto targs = std::make_tuple(
            impl::wrap_arg(std::forward<Args>(args))...);
        const std::size_t expected_format_size = impl::format_impl(
            nullptr, 0, fmt, targs);
        vector<char> buffer(expected_format_size + 1, '\0');
        const std::size_t actual_format_size = impl::format_impl(
            buffer.data(), buffer.size(), fmt, targs);
        E2D_ASSERT(expected_format_size == actual_format_size);
        return str(buffer.data(), buffer.data() + actual_format_size);
    }

    template < typename... Args >
    bool rformat_nothrow(str& dst, str_view fmt, Args&&... args) noexcept {
        try {
            dst = rformat(fmt, std::forward<Args>(args)...);
            return true;
        } catch (...) {
            return false;
        }
    }
}
