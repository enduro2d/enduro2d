/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/streams.hpp>

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
                }
                pos_ -= uoffset;
                return pos_;
            } else {
                const std::size_t uoffset = math::abs_to_unsigned(offset);
                const std::size_t available_bytes = relative
                    ? data_.size() - pos_
                    : data_.size();
                if ( uoffset > available_bytes ) {
                    throw bad_stream_operation();
                }
                pos_ = uoffset + (data_.size() - available_bytes);
                return pos_;
            }
        }

        std::size_t tell() const final {
            return pos_;
        }

        std::size_t length() const noexcept final {
            return data_.size();
        }
    private:
        buffer data_;
        std::size_t pos_ = 0;
    };
}

namespace e2d
{
    //
    // input_sequence
    //

    input_sequence::input_sequence(input_stream& stream) noexcept
    : stream_(stream) {}

    bool input_sequence::success() const noexcept {
        return success_;
    }

    std::exception_ptr input_sequence::exception() const noexcept {
        return exception_;
    }

    input_sequence& input_sequence::seek(std::ptrdiff_t offset, bool relative) noexcept {
        try {
            if ( success_ ) {
                stream_.seek(offset, relative);
            }
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    input_sequence& input_sequence::read(void* dst, std::size_t size) noexcept {
        try {
            success_ = success_ && stream_.read(dst, size) == size;
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    input_sequence& input_sequence::read_all(str& dst) noexcept {
        try {
            if ( success_ ) {
                vector<char> tail(stream_.length() - stream_.tell());
                if ( tail.size() == stream_.read(tail.data(), tail.size()) ) {
                    dst.assign(tail.data(), tail.data() + tail.size());
                } else {
                    success_ = false;
                }
            }
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    input_sequence& input_sequence::read_all(buffer& dst) noexcept {
        try {
            if ( success_ ) {
                buffer tail(stream_.length() - stream_.tell());
                if ( tail.size() == stream_.read(tail.data(), tail.size()) ) {
                    tail.swap(dst);
                } else {
                    success_ = false;
                }
            }
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    //
    // output_sequence
    //

    output_sequence::output_sequence(output_stream& stream) noexcept
    : stream_(stream) {}

    bool output_sequence::success() const noexcept {
        return success_;
    }

    std::exception_ptr output_sequence::exception() const noexcept {
        return exception_;
    }

    output_sequence& output_sequence::seek(std::ptrdiff_t offset, bool relative) noexcept {
        try {
            if ( success_ ) {
                stream_.seek(offset, relative);
            }
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    output_sequence& output_sequence::write(const void* src, std::size_t size) noexcept {
        try {
            success_ = success_ && stream_.write(src, size) == size;
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    output_sequence& output_sequence::write_all(str_view src) noexcept {
        return success_
            ? write(src.data(), src.size())
            : *this;
    }

    output_sequence& output_sequence::write_all(buffer_view src) noexcept {
        return success_
            ? write(src.data(), src.size())
            : *this;
    }

    output_sequence& output_sequence::flush() noexcept {
        try {
            if ( success_ ) {
                stream_.flush();
            }
        } catch (...) {
            success_ = false;
            exception_ = std::current_exception();
        }
        return *this;
    }

    output_sequence& output_sequence::flush_if(bool yesno) noexcept {
        return yesno ? flush() : *this;
    }
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

namespace e2d::streams
{
    bool try_read_tail(str& dst, const input_stream_uptr& stream) noexcept {
        return stream
            ? input_sequence(*stream)
                .read_all(dst)
                .success()
            : false;
    }

    bool try_read_tail(buffer& dst, const input_stream_uptr& stream) noexcept {
        return stream
            ? input_sequence(*stream)
                .read_all(dst)
                .success()
            : false;
    }

    bool try_write_tail(str_view src, const output_stream_uptr& stream) noexcept {
        return stream
            ? output_sequence(*stream)
                .write_all(src)
                .success()
            : false;
    }

    bool try_write_tail(buffer_view src, const output_stream_uptr& stream) noexcept {
        return stream
            ? output_sequence(*stream)
                .write_all(src)
                .success()
            : false;
    }
}
