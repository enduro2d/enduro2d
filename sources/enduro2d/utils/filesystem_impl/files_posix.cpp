/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "files.hpp"

#if defined(E2D_FILES_MODE) && E2D_FILES_MODE == E2D_FILES_MODE_POSIX

#include <fcntl.h>
#include <unistd.h>

namespace
{
    using namespace e2d;

    class read_file_posix final : public read_file {
    public:
        read_file_posix(str path)
        : path_(std::move(path))
        {
            if ( !open_() ) {
                throw bad_stream_operation();
            }
        }

        ~read_file_posix() noexcept final {
            close_();
        }
    public:
        std::size_t read(void* dst, std::size_t size) final {
            E2D_ASSERT(is_opened_());
            const std::ptrdiff_t rread = ::read(handle_, dst, size);
            return rread >= 0
                ? math::numeric_cast<std::size_t>(rread)
                : throw bad_stream_operation();
        }

        std::size_t seek(std::ptrdiff_t offset, bool relative) final {
            E2D_ASSERT(is_opened_());
            const std::ptrdiff_t rlseek = ::lseek(handle_, offset, relative ? SEEK_CUR : SEEK_SET);
            return rlseek >= 0
                ? math::numeric_cast<std::size_t>(rlseek)
                : throw bad_stream_operation();
        }

        std::size_t tell() const final {
            E2D_ASSERT(is_opened_());
            const std::ptrdiff_t rlseek = ::lseek(handle_, 0, SEEK_CUR);
            return rlseek >= 0
                ? math::numeric_cast<std::size_t>(rlseek)
                : throw bad_stream_operation();
        }

        std::size_t length() const noexcept final {
            return length_;
        }

        const str& path() const noexcept final {
            return path_;
        }
    private:
        bool open_() noexcept {
            E2D_ASSERT(!is_opened_());
            handle_ = ::open(path_.c_str(), O_RDONLY);
            if ( handle_ < 0 ) {
                close_();
                return false;
            }
            if ( ::lseek(handle_, 0, SEEK_END) < 0 ) {
                close_();
                return false;
            }
            const std::ptrdiff_t rlseek = ::lseek(handle_, 0, SEEK_CUR);
            if ( rlseek < 0 ) {
                close_();
                return false;
            }
            length_ = math::numeric_cast<std::size_t>(rlseek);
            if ( ::lseek(handle_, 0, SEEK_SET) < 0 ) {
                close_();
                return false;
            }
            return true;
        }

        void close_() noexcept {
            if ( handle_ >= 0 ) {
                ::close(handle_);
                handle_ = -1;
            }
        }

        bool is_opened_() const noexcept {
            return handle_ >= 0;
        }
    private:
        str path_;
        std::size_t length_ = 0;
        int handle_ = -1;
    };

    class write_file_posix : public write_file {
    public:
        write_file_posix(str path, bool append)
        : path_(std::move(path))
        {
            if ( !open_(append) ) {
                throw bad_stream_operation();
            }
        }

        ~write_file_posix() noexcept final {
            close_();
        }
    public:
        std::size_t write(const void* src, std::size_t size) final {
            E2D_ASSERT(is_opened_());
            const std::ptrdiff_t rwrite = ::write(handle_, src, size);
            return rwrite >= 0
                ? math::numeric_cast<std::size_t>(rwrite)
                : throw bad_stream_operation();
        }

        std::size_t seek(std::ptrdiff_t offset, bool relative) final {
            E2D_ASSERT(is_opened_());
            const std::ptrdiff_t rlseek = ::lseek(handle_, offset, relative ? SEEK_CUR : SEEK_SET);
            return rlseek >= 0
                ? math::numeric_cast<std::size_t>(rlseek)
                : throw bad_stream_operation();
        }

        std::size_t tell() const final {
            E2D_ASSERT(is_opened_());
            const std::ptrdiff_t rlseek = ::lseek(handle_, 0, SEEK_CUR);
            return rlseek >= 0
                ? math::numeric_cast<std::size_t>(rlseek)
                : throw bad_stream_operation();
        }

        void flush() const final {
            E2D_ASSERT(is_opened_());
            if ( 0 != ::fsync(handle_) ) {
                throw bad_stream_operation();
            }
        }

        const str& path() const noexcept final {
            return path_;
        }
    private:
        bool open_(bool append) noexcept {
            E2D_ASSERT(!is_opened_());
            handle_ = ::open(
                path_.c_str(),
                append ? (O_WRONLY|O_CREAT) : (O_WRONLY|O_CREAT|O_TRUNC),
                S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
            if ( handle_ < 0 ) {
                close_();
                return false;
            }
            if ( ::lseek(handle_, 0, SEEK_END) < 0 ) {
                close_();
                return false;
            }
            return true;
        }

        void close_() noexcept {
            if ( handle_ >= 0 ) {
                ::close(handle_);
                handle_ = -1;
            }
        }

        bool is_opened_() const noexcept {
            return handle_ >= 0;
        }
    private:
        str path_;
        int handle_ = -1;
    };
}

namespace e2d::impl
{
    read_file_uptr make_read_file(str_view path) noexcept {
        try {
            return std::make_unique<read_file_posix>(str(path));
        } catch (...) {
            return nullptr;
        }
    }

    write_file_uptr make_write_file(str_view path, bool append) noexcept {
        try {
            return std::make_unique<write_file_posix>(str(path), append);
        } catch (...) {
            return nullptr;
        }
    }
}

#endif
