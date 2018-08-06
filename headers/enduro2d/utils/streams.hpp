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
        virtual std::size_t length() const = 0;
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
