/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "filesystem.hpp"

namespace e2d::filesystem
{
    template < typename Iter >
    bool extract_directory(str_view path, Iter result_iter) {
        return trace_directory(path, [&result_iter](str_view filename, bool directory){
            *result_iter++ = std::pair<str,bool>{filename, directory};
            return true;
        });
    }

    template < typename Iter >
    bool extract_directory_recursive(str_view path, Iter result_iter) {
        return trace_directory_recursive(path, [&result_iter](str_view filename, bool directory){
            *result_iter++ = std::pair<str,bool>{filename, directory};
            return true;
        });
    }
}
