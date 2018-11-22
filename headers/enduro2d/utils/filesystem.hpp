/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_B13171AF5FD34D6284271BD2F4B1F6DD
#define E2D_INCLUDE_GUARD_B13171AF5FD34D6284271BD2F4B1F6DD
#pragma once

#include "_utils.hpp"
#include "streams.hpp"

namespace e2d
{
    class read_file : public input_stream {
    public:
        virtual const str& path() const noexcept = 0;
    };
    using read_file_uptr = std::unique_ptr<read_file>;

    class write_file : public output_stream {
    public:
        virtual const str& path() const noexcept = 0;
    };
    using write_file_uptr = std::unique_ptr<write_file>;
}

namespace e2d
{
    read_file_uptr make_read_file(str_view path) noexcept;
    write_file_uptr make_write_file(str_view path, bool append) noexcept;
}

namespace e2d { namespace filesystem
{
    bool remove(str_view path);
    bool exists(str_view path);

    bool remove_file(str_view path);
    bool remove_directory(str_view path);

    bool file_exists(str_view path);
    bool directory_exists(str_view path);

    bool create_file(str_view path);
    bool create_directory(str_view path);
    bool create_directory_recursive(str_view path);

    using trace_func = std::function<bool(
        str_view relative,
        bool directory)>;

    bool trace_directory(
        str_view path,
        const trace_func& func);

    bool trace_directory_recursive(
        str_view path,
        const trace_func& func);

    template < typename Iter >
    bool extract_directory(
        str_view path,
        Iter result_iter);

    template < typename Iter >
    bool extract_directory_recursive(
        str_view path,
        Iter result_iter);

    bool try_read_all(str& dst, str_view path) noexcept;
    bool try_read_all(buffer& dst, str_view path) noexcept;

    bool try_write_all(const str& src, str_view path, bool append) noexcept;
    bool try_write_all(const buffer& src, str_view path, bool append) noexcept;

    enum class predef_path {
        home,
        appdata,
        desktop,
        working,
        documents,
        resources,
        executable
    };

    bool extract_predef_path(str& dst, predef_path path_type);
}}

#include "filesystem.inl"
#endif
