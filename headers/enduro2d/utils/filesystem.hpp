/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "buffer_view.hpp"
#include "streams.hpp"

namespace e2d
{
    class read_file;
    using read_file_uptr = std::unique_ptr<read_file>;

    class read_file : public input_stream {
    public:
        virtual const str& path() const noexcept = 0;
    };

    class write_file;
    using write_file_uptr = std::unique_ptr<write_file>;

    class write_file : public output_stream {
    public:
        virtual const str& path() const noexcept = 0;
    };
}

namespace e2d
{
    read_file_uptr make_read_file(str_view path) noexcept;
    write_file_uptr make_write_file(str_view path, bool append) noexcept;
}

namespace e2d::filesystem
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
    bool try_write_all(str_view src, str_view path, bool append) noexcept;
    bool try_write_all(buffer_view src, str_view path, bool append) noexcept;

    ENUM_HPP_CLASS_DECL(predef_path, u8,
        (home)
        (appdata)
        (desktop)
        (working)
        (documents)
        (resources)
        (executable))
    ENUM_HPP_REGISTER_TRAITS(predef_path)

    bool extract_predef_path(str& dst, predef_path path_type);
}

#include "filesystem.inl"
