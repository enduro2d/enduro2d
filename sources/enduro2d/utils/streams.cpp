/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/utils/streams.hpp>
#include <enduro2d/utils/buffer.hpp>

namespace
{
    using namespace e2d;

    class memory_stream final : public input_stream {
    public:
        memory_stream(buffer data) noexcept
        : data_(std::move(data)) {}

        std::size_t read(void* dst, std::size_t size) final {
            const std::size_t read_bytes = dst
                ? math::min(size, data_.size() - pos_)
                : 0;
            if ( read_bytes > 0 ) {
                std::memcpy(dst, data_.data() + pos_, read_bytes);
                pos_ += read_bytes;
            }
            return read_bytes;
        }

        std::size_t seek(std::ptrdiff_t offset, bool relative) final {
            if ( offset < 0 ) {
                const std::size_t uoffset = math::abs_to_unsigned(offset);
                if ( !relative || uoffset > pos_ ) {
                    throw bad_stream_operation();
                } else {
                    pos_ -= uoffset;
                    return pos_;
                }
            } else {
                const std::size_t uoffset = math::abs_to_unsigned(offset);
                const std::size_t available_bytes = relative
                    ? data_.size() - pos_
                    : data_.size();
                if ( uoffset > available_bytes ) {
                    throw bad_stream_operation();
                } else {
                    pos_ = uoffset + (data_.size() - available_bytes);
                    return pos_;
                }
            }
        }

        std::size_t tell() const final {
            return pos_;
        }

        std::size_t length() const final {
            return data_.size();
        }
    private:
        buffer data_;
        std::size_t pos_ = 0;
    };
}

namespace e2d
{
    input_stream_uptr make_memory_stream(buffer data) noexcept {
        try {
            return std::make_unique<memory_stream>(std::move(data));
        } catch (...) {
            return nullptr;
        }
    }
}

namespace e2d { namespace streams
{
    bool try_read_tail(buffer& dst, const input_stream_uptr& stream) noexcept {
        try {
            if ( stream ) {
                buffer tail(stream->length() - stream->tell());
                if ( tail.size() == stream->read(tail.data(), tail.size()) ) {
                    dst.swap(tail);
                    return true;
                }
            }
        } catch (...) {
            // nothing
        }
        return false;
    }

    bool try_write_tail(const buffer& src, const output_stream_uptr& stream) noexcept {
        try {
            return
                stream &&
                src.size() == stream->write(src.data(), src.size());
        } catch (...) {
            return false;
        }
    }
}}
