/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class bad_stream_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad stream operation";
        }
    };

    class input_stream : private noncopyable {
    public:
        virtual ~input_stream() noexcept = default;
        virtual std::size_t read(void* dst, std::size_t size) = 0;
        virtual std::size_t seek(std::ptrdiff_t offset, bool relative) = 0;
        virtual std::size_t tell() const = 0;
        virtual std::size_t length() const noexcept = 0;
    };
    using input_stream_uptr = std::unique_ptr<input_stream>;

    class output_stream : private noncopyable {
    public:
        virtual ~output_stream() noexcept = default;
        virtual std::size_t write(const void* src, std::size_t size) = 0;
        virtual std::size_t seek(std::ptrdiff_t offset, bool relative) = 0;
        virtual std::size_t tell() const = 0;
    };
    using output_stream_uptr = std::unique_ptr<output_stream>;
}

namespace e2d
{
    class input_sequence : private noncopyable {
    public:
        input_sequence(input_stream& stream) noexcept
        : stream_(stream) {}

        bool success() const noexcept {
            return success_;
        }

        std::exception_ptr exception() const noexcept {
            return exception_;
        }

        input_sequence& seek(std::ptrdiff_t offset, bool relative) noexcept {
            try {
                stream_.seek(offset, relative);
            } catch (...) {
                success_ = false;
                exception_ = std::current_exception();
            }
            return *this;
        }

        template < typename T >
        std::enable_if_t<
            std::is_arithmetic<T>::value,
            input_sequence&>
        read(T& v) noexcept {
            return read(&v, sizeof(v));
        }

        input_sequence& read(void* dst, std::size_t size) noexcept {
            try {
                success_ = success_ && stream_.read(dst, size) == size;
            } catch (...) {
                success_ = false;
                exception_ = std::current_exception();
            }
            return *this;
        }
    private:
        input_stream& stream_;
        bool success_ = true;
        std::exception_ptr exception_ = nullptr;
    };

    class output_sequence : private noncopyable {
    public:
        output_sequence(output_stream& stream) noexcept
        : stream_(stream) {}

        bool success() const noexcept {
            return success_;
        }

        std::exception_ptr exception() const noexcept {
            return exception_;
        }

        output_sequence& seek(std::ptrdiff_t offset, bool relative) noexcept {
            try {
                stream_.seek(offset, relative);
            } catch (...) {
                success_ = false;
                exception_ = std::current_exception();
            }
            return *this;
        }

        template < typename T >
        std::enable_if_t<
            std::is_arithmetic<T>::value,
            output_sequence&>
        write(T v) noexcept {
            return write(&v, sizeof(v));
        }

        output_sequence& write(const void* src, std::size_t size) noexcept {
            try {
                success_ = success_ && stream_.write(src, size) == size;
            } catch (...) {
                success_ = false;
                exception_ = std::current_exception();
            }
            return *this;
        }
    private:
        output_stream& stream_;
        bool success_ = true;
        std::exception_ptr exception_ = nullptr;
    };
}

namespace e2d
{
    input_stream_uptr make_memory_stream(buffer data) noexcept;
}

namespace e2d { namespace streams
{
    bool try_read_tail(
        buffer& dst,
        const input_stream_uptr& stream) noexcept;

    bool try_write_tail(
        const buffer& src,
        const output_stream_uptr& stream) noexcept;
}}
