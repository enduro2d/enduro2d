/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "filesystem_impl.hpp"

#if defined(E2D_FILESYSTEM_MODE) && E2D_FILESYSTEM_MODE == E2D_FILESYSTEM_MODE_WINAPI

#include <windows.h>

namespace
{
    using namespace e2d;

    class read_file_winapi : public read_file {
    public:
        read_file_winapi(str path)
        : path_(std::move(path))
        {
            if ( !open_() ) {
                throw bad_stream_operation();
            }
        }

        ~read_file_winapi() noexcept final {
            close_();
        }
    public:
        std::size_t read(void* dst, std::size_t size) final {
            E2D_ASSERT(is_opened_());
            DWORD read_bytes = 0;
            const BOOL rread = ::ReadFile(
                handle_,
                dst,
                math::numeric_cast<DWORD>(size),
                &read_bytes,
                NULL);
            return TRUE == rread
                ? math::numeric_cast<std::size_t>(read_bytes)
                : throw bad_stream_operation();
        }

        std::size_t seek(std::ptrdiff_t offset, bool relative) final {
            E2D_ASSERT(is_opened_());
            const DWORD rseek = ::SetFilePointer(
                handle_,
                math::numeric_cast<LONG>(offset),
                NULL,
                relative ? FILE_CURRENT : FILE_BEGIN);
            return rseek != INVALID_SET_FILE_POINTER
                ? math::numeric_cast<std::size_t>(rseek)
                : throw bad_stream_operation();
        }

        std::size_t tell() const final {
            E2D_ASSERT(is_opened_());
            const DWORD rseek = ::SetFilePointer(handle_, 0, NULL, FILE_CURRENT);
            return rseek != INVALID_SET_FILE_POINTER
                ? math::numeric_cast<std::size_t>(rseek)
                : throw bad_stream_operation();
        }

        std::size_t length() const final {
            return length_;
        }

        const str& path() const noexcept final {
            return path_;
        }
    private:
        bool open_() {
            E2D_ASSERT(!is_opened_());
            const wstr wide_path = make_wide(path_);
            handle_ = ::CreateFileW(
                wide_path.c_str(),
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_READONLY,
                NULL);
            if ( INVALID_HANDLE_VALUE == handle_ ) {
                handle_ = ::CreateFileW(
                    wide_path.c_str(),
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_READONLY,
                    NULL);
            }
            if ( INVALID_HANDLE_VALUE == handle_ ) {
                close_();
                return false;
            }
            const DWORD file_size = ::GetFileSize(handle_, NULL);
            if ( INVALID_FILE_SIZE == file_size ) {
                close_();
                return false;
            }
            length_ = math::numeric_cast<std::size_t>(file_size);
            return true;
        }

        void close_() noexcept {
            if ( INVALID_HANDLE_VALUE != handle_ ) {
                ::CloseHandle(handle_);
                handle_ = INVALID_HANDLE_VALUE;
            }
        }

        bool is_opened_() const noexcept {
            return INVALID_HANDLE_VALUE != handle_;
        }
    private:
        str path_;
        std::size_t length_ = 0;
        HANDLE handle_ = INVALID_HANDLE_VALUE;
    };

    class write_file_winapi : public write_file {
    public:
        write_file_winapi(str path, bool append)
        : path_(std::move(path))
        {
            if ( !open_(append) ) {
                throw bad_stream_operation();
            }
        }

        ~write_file_winapi() noexcept final {
            close_();
        }
    public:
        std::size_t write(const void* src, std::size_t size) final {
            E2D_ASSERT(is_opened_());
            DWORD write_bytes = 0;
            const BOOL rwrite = ::WriteFile(
                handle_,
                src,
                math::numeric_cast<DWORD>(size),
                &write_bytes,
                NULL);
            return TRUE == rwrite
                ? math::numeric_cast<std::size_t>(write_bytes)
                : throw bad_stream_operation();
        }

        std::size_t seek(std::ptrdiff_t offset, bool relative) final {
            E2D_ASSERT(is_opened_());
            const DWORD rseek = ::SetFilePointer(
                handle_,
                math::numeric_cast<LONG>(offset),
                NULL,
                relative ? FILE_CURRENT : FILE_BEGIN);
            return rseek != INVALID_SET_FILE_POINTER
                ? math::numeric_cast<std::size_t>(rseek)
                : throw bad_stream_operation();
        }

        std::size_t tell() const final {
            E2D_ASSERT(is_opened_());
            const DWORD rseek = ::SetFilePointer(handle_, 0, NULL, FILE_CURRENT);
            return rseek != INVALID_SET_FILE_POINTER
                ? math::numeric_cast<std::size_t>(rseek)
                : throw bad_stream_operation();
        }

        const str& path() const noexcept final {
            return path_;
        }
    private:
        bool open_(bool append) {
            E2D_ASSERT(!is_opened_());
            const wstr wide_path = make_wide(path_);
            handle_ = ::CreateFileW(
                wide_path.c_str(),
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                append ? OPEN_ALWAYS : CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            if ( INVALID_HANDLE_VALUE == handle_ ) {
                close_();
                return false;
            }
            if ( append && INVALID_SET_FILE_POINTER == ::SetFilePointer(
                handle_,
                0,
                NULL,
                FILE_END) )
            {
                close_();
                return false;
            }
            return true;
        }

        void close_() noexcept {
            if ( INVALID_HANDLE_VALUE != handle_ ) {
                ::CloseHandle(handle_);
                handle_ = INVALID_HANDLE_VALUE;
            }
        }

        bool is_opened_() const noexcept {
            return INVALID_HANDLE_VALUE != handle_;
        }
    private:
        str path_;
        HANDLE handle_ = INVALID_HANDLE_VALUE;
    };
}

namespace e2d { namespace impl
{
    read_file_uptr make_read_file(str_view path) noexcept {
        try {
            return std::make_unique<read_file_winapi>(path);
        } catch (...) {
            return nullptr;
        }
    }

    write_file_uptr make_write_file(str_view path, bool append) noexcept {
        try {
            return std::make_unique<write_file_winapi>(path, append);
        } catch (...) {
            return nullptr;
        }
    }
}}

#endif
