/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "filesystem_impl/files.hpp"
#include "filesystem_impl/filesystem.hpp"

namespace e2d
{
    read_file_uptr make_read_file(str_view path) noexcept {
        return impl::make_read_file(path);
    }

    write_file_uptr make_write_file(str_view path, bool append) noexcept {
        return impl::make_write_file(path, append);
    }
}

namespace e2d { namespace filesystem
{
    bool remove(str_view path) {
        return remove_file(path)
            || remove_directory(path);
    }

    bool exists(str_view path) {
        return file_exists(path)
            || directory_exists(path);
    }

    bool remove_file(str_view path) {
        return impl::remove_file(path);
    }

    bool remove_directory(str_view path) {
        const std::function<bool(str_view, bool)> rfunc =
        [&path](str_view relative, bool directory){
            const str fullpath = path::combine(path, relative);
            return (directory && remove_directory(fullpath))
                || (!directory && remove_file(fullpath));
        };
        return trace_directory(path, rfunc)
            && impl::remove_directory(path);
    }

    bool file_exists(str_view path) {
        return impl::file_exists(path);
    }

    bool directory_exists(str_view path) {
        return impl::directory_exists(path);
    }

    bool create_file(str_view path) {
        return create_directory_recursive(path::parent_path(path))
            && make_write_file(path, true);
    }

    bool create_directory(str_view path) {
        return impl::create_directory(path);
    }

    bool create_directory_recursive(str_view path) {
        if ( path.empty() || directory_exists(path) ) {
            return true;
        }
        if ( !create_directory_recursive(path::parent_path(path)) ) {
            return false;
        }
        return impl::create_directory(path);
    }

    bool trace_directory(str_view path, const trace_func& func) {
        return impl::trace_directory(path, func);
    }

    bool trace_directory_recursive(str_view path, const trace_func& func) {
        const std::function<bool(str_view, str_view, bool)> rfunc =
        [&path, &func, &rfunc](str_view parent, str_view relative, bool directory) {
            const str filename = path::combine(parent, relative);
            if ( !func || !func(filename, directory) ) {
                return false;
            }
            if ( directory ) {
                return impl::trace_directory(
                    path::combine(path, filename),
                    std::bind(rfunc, filename, std::placeholders::_1, std::placeholders::_2));
            }
            return true;
        };
        return impl::trace_directory(
            path,
            std::bind(rfunc, "", std::placeholders::_1, std::placeholders::_2));
    }

    bool try_read_all(str& dst, str_view path) noexcept {
        return streams::try_read_tail(
            dst, make_read_file(path));
    }

    bool try_read_all(buffer& dst, str_view path) noexcept {
        return streams::try_read_tail(
            dst, make_read_file(path));
    }

    bool try_write_all(const str& src, str_view path, bool append) noexcept {
        return streams::try_write_tail(
            src, make_write_file(path, append));
    }

    bool try_write_all(const buffer& src, str_view path, bool append) noexcept {
        return streams::try_write_tail(
            src, make_write_file(path, append));
    }

    bool extract_predef_path(str& dst, predef_path path_type) {
        return impl::extract_predef_path(dst, path_type);
    }
}}
