/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class bad_vfs_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad vfs operation";
        }
    };

    class vfs final : public module<vfs> {
    public:
        vfs();
        ~vfs() noexcept final;

        class file_source : private e2d::noncopyable {
        public:
            virtual ~file_source() noexcept = default;
            virtual bool valid() const noexcept = 0;
            virtual bool exists(str_view path) const = 0;
            virtual input_stream_uptr open(str_view path) const = 0;
            virtual std::pair<buffer,bool> load(str_view path) const = 0;
            virtual output_stream_uptr write(str_view path, bool append) const = 0;
            virtual bool trace(str_view path, filesystem::trace_func func) const = 0;
        };
        using file_source_uptr = std::unique_ptr<file_source>;

        template < typename T, typename... Args >
        bool register_scheme(str_view scheme, Args&&... args);
        bool register_scheme(str_view scheme, file_source_uptr source);
        bool unregister_scheme(str_view scheme);

        bool register_scheme_alias(str_view scheme, url alias);
        bool unregister_scheme_alias(str_view scheme);

        bool exists(const url& url) const;
        input_stream_uptr open(const url& url) const;
        std::pair<buffer,bool> load(const url& url) const;
        output_stream_uptr write(const url& url, bool append) const;
        stdex::promise<std::pair<buffer,bool>> load_async(const url& url) const;

        template < typename Iter >
        bool extract(const url& url, Iter result_iter) const;
        bool trace(const url& url, filesystem::trace_func func) const;

        url resolve_scheme_aliases(const url& url) const;
    private:
        class state;
        std::unique_ptr<state> state_;
    };

    class archive_file_source final : public vfs::file_source {
    public:
        archive_file_source(input_stream_uptr stream);
        ~archive_file_source() noexcept final;
        bool valid() const noexcept final;
        bool exists(str_view path) const final;
        input_stream_uptr open(str_view path) const final;
        std::pair<buffer,bool> load(str_view path) const final;
        output_stream_uptr write(str_view path, bool append) const final;
        bool trace(str_view path, filesystem::trace_func func) const final;
    private:
        class state;
        std::unique_ptr<state> state_;
    };

    class filesystem_file_source final : public vfs::file_source {
    public:
        filesystem_file_source();
        ~filesystem_file_source() noexcept final;
        bool valid() const noexcept final;
        bool exists(str_view path) const final;
        input_stream_uptr open(str_view path) const final;
        std::pair<buffer,bool> load(str_view path) const final;
        output_stream_uptr write(str_view path, bool append) const final;
        bool trace(str_view path, filesystem::trace_func func) const final;
    };
}

namespace e2d
{
    template < typename T, typename... Args >
    bool vfs::register_scheme(str_view scheme, Args&&... args) {
        return register_scheme(
            scheme,
            std::make_unique<T>(std::forward<Args>(args)...));
    }

    template < typename Iter >
    bool vfs::extract(const url& url, Iter result_iter) const {
        return trace(url, [&result_iter](str_view filename, bool directory){
            *result_iter++ = std::pair<str,bool>{filename, directory};
            return true;
        });
    }
}
