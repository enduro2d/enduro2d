/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/path.hpp>

namespace
{
    using namespace e2d;

    const str_view dot = ".";
    const str_view dot_dot = "..";
    const str_view path_separators = "/\\";

    bool is_directory_separator(char ch) noexcept {
        return path_separators.cend() != std::find(
            path_separators.cbegin(), path_separators.cend(), ch);
    }

    str str_view_concat(str_view v1, str_view v2) {
        str result;
        result.reserve(v1.size() + v2.size());
        result.append(v1.cbegin(), v1.cend());
        result.append(v2.cbegin(), v2.cend());
        return result;
    }

    str str_view_concat(str_view v1, str_view v2, str_view v3) {
        str result;
        result.reserve(v1.size() + v2.size() + v3.size());
        result.append(v1.cbegin(), v1.cend());
        result.append(v2.cbegin(), v2.cend());
        result.append(v3.cbegin(), v3.cend());
        return result;
    }
}

namespace e2d { namespace path
{
    str combine(str_view lhs, str_view rhs) {
        if ( lhs.empty() || is_absolute(rhs) ) {
            return str(rhs);
        }
        if ( rhs.empty() ) {
            return str(lhs);
        }
        return is_directory_separator(lhs.back())
            ? str_view_concat(lhs, rhs)
            : str_view_concat(lhs, "/", rhs);
    }

    str remove_filename(str_view path) {
        const str name = filename(path);
        return name.empty()
            ? str(path)
            : str(path.substr(0, path.length() - name.length()));
    }

    str remove_extension(str_view path) {
        const str ext = extension(path);
        return ext.empty()
            ? str(path)
            : str(path.substr(0, path.length() - ext.length()));
    }

    str replace_filename(str_view path, str_view filename) {
        const str without_name = remove_filename(path);
        return str_view_concat(without_name, filename);
    }

    str replace_extension(str_view path, str_view extension) {
        const str without_ext = remove_extension(path);
        return extension.empty()
            ? without_ext
            : (extension.front() == '.'
                ? str_view_concat(without_ext, extension)
                : str_view_concat(without_ext, dot, extension));
    }

    str stem(str_view path) {
        const str name = filename(path);
        if ( name.empty() || name == dot || name == dot_dot ) {
            return name;
        }
        const str::size_type ext_pos = name.rfind('.');
        return ext_pos != str::npos
            ? name.substr(0, ext_pos)
            : name;
    }

    str filename(str_view path) {
        const auto sep_e = std::find_if(
            path.crbegin(), path.crend(), &is_directory_separator);
        const auto sep_d = std::distance(sep_e, path.crend());
        return str(path.substr(static_cast<std::size_t>(sep_d)));
    }

    str extension(str_view path) {
        const str name = filename(path);
        if ( name.empty() || name == dot || name == dot_dot ) {
            return str();
        }
        const str::size_type ext_pos = name.rfind('.');
        return ext_pos != str::npos
            ? name.substr(ext_pos)
            : str();
    }

    str parent_path(str_view path) {
        const auto sep_e = std::find_if(
            path.crbegin(), path.crend(), &is_directory_separator);
        if ( sep_e == path.crend() ) {
            return str();
        }
        const auto sep_b = std::find_if_not(
            sep_e, path.crend(), &is_directory_separator);
        if ( sep_b == path.crend() ) {
            return str();
        }
        const auto sep_d = std::distance(sep_b, path.crend());
        return str(path.substr(0, static_cast<std::size_t>(sep_d)));
    }

    bool is_absolute(str_view path) noexcept {
        return
            (path.size() >= 1 && (path[0] == '/' || path[0] == '\\')) ||
            (path.size() >= 2 && path[1] == ':');
    }

    bool is_relative(str_view path) noexcept {
        return !is_absolute(path);
    }
}}
