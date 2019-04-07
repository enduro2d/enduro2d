/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

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
