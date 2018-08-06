/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

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

namespace e2d { namespace path
{
    str combine(str_view lhs, str_view rhs);

    str remove_filename(str_view path);
    str remove_extension(str_view path);

    str replace_filename(str_view path, str_view filename);
    str replace_extension(str_view path, str_view extension);

    str stem(str_view path);
    str filename(str_view path);
    str extension(str_view path);
    str parent_path(str_view path);

    bool is_absolute(str_view path) noexcept;
    bool is_relative(str_view path) noexcept;
}}


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

    bool try_read_all(buffer& dst, str_view path) noexcept;
    bool try_write_all(const buffer& src, str_view path, bool append) noexcept;
}}
