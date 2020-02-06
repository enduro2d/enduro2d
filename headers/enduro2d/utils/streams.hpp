/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "buffer_view.hpp"

namespace e2d
{
    class bad_stream_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad stream operation";
        }
    };

    class input_stream;
    using input_stream_uptr = std::unique_ptr<input_stream>;

    class input_stream : private noncopyable {
    public:
        virtual ~input_stream() noexcept = default;
        virtual std::size_t read(void* dst, std::size_t size) = 0;
        virtual std::size_t seek(std::ptrdiff_t offset, bool relative) = 0;
        virtual std::size_t tell() const = 0;
        virtual std::size_t length() const noexcept = 0;
    };

    class output_stream;
    using output_stream_uptr = std::unique_ptr<output_stream>;

    class output_stream : private noncopyable {
    public:
        virtual ~output_stream() noexcept = default;
        virtual std::size_t write(const void* src, std::size_t size) = 0;
        virtual std::size_t seek(std::ptrdiff_t offset, bool relative) = 0;
        virtual std::size_t tell() const = 0;
        virtual void flush() const = 0;
    };
}

namespace e2d
{
    class input_sequence final : private noncopyable {
    public:
        input_sequence(input_stream& stream) noexcept;
        ~input_sequence() noexcept = default;

        bool success() const noexcept;
        std::exception_ptr exception() const noexcept;

        input_sequence& seek(std::ptrdiff_t offset, bool relative) noexcept;
        input_sequence& read(void* dst, std::size_t size) noexcept;
        input_sequence& read_all(str& dst) noexcept;
        input_sequence& read_all(buffer& dst) noexcept;

        template < typename T >
        std::enable_if_t<
            std::is_arithmetic<T>::value,
            input_sequence&>
        read(T& v) noexcept;
    private:
        input_stream& stream_;
        bool success_{true};
        std::exception_ptr exception_{nullptr};
    };

    class output_sequence final : private noncopyable {
    public:
        output_sequence(output_stream& stream) noexcept;
        ~output_sequence() noexcept = default;

        bool success() const noexcept;
        std::exception_ptr exception() const noexcept;

        output_sequence& seek(std::ptrdiff_t offset, bool relative) noexcept;
        output_sequence& write(const void* src, std::size_t size) noexcept;
        output_sequence& write_all(str_view src) noexcept;
        output_sequence& write_all(buffer_view src) noexcept;

        output_sequence& flush() noexcept;
        output_sequence& flush_if(bool yesno) noexcept;

        template < typename T >
        std::enable_if_t<
            std::is_arithmetic<T>::value,
            output_sequence&>
        write(T v) noexcept;
    private:
        output_stream& stream_;
        bool success_{true};
        std::exception_ptr exception_{nullptr};
    };
}

namespace e2d
{
    input_stream_uptr make_memory_stream(buffer data) noexcept;
}

namespace e2d::streams
{
    bool try_read_tail(
        str& dst,
        const input_stream_uptr& stream) noexcept;

    bool try_read_tail(
        buffer& dst,
        const input_stream_uptr& stream) noexcept;

    bool try_write_tail(
        str_view src,
        const output_stream_uptr& stream) noexcept;

    bool try_write_tail(
        buffer_view src,
        const output_stream_uptr& stream) noexcept;
}

#include "streams.inl"
