/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_ADA6066FA64F4B30BE65FE6AFB3D887F
#define E2D_INCLUDE_GUARD_ADA6066FA64F4B30BE65FE6AFB3D887F
#pragma once

#include "_utils.hpp"
#include "filesystem.hpp"

namespace e2d { namespace filesystem
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
}}

#endif
