/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/path.hpp>
#include <enduro2d/utils/strings.hpp>
#include <enduro2d/utils/filesystem.hpp>

#define E2D_FILESYSTEM_MODE_POSIX  1
#define E2D_FILESYSTEM_MODE_WINAPI 2

#ifndef E2D_FILESYSTEM_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_ANDROID
#    define E2D_FILESYSTEM_MODE E2D_FILESYSTEM_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_FILESYSTEM_MODE E2D_FILESYSTEM_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_FILESYSTEM_MODE E2D_FILESYSTEM_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_FILESYSTEM_MODE E2D_FILESYSTEM_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_FILESYSTEM_MODE E2D_FILESYSTEM_MODE_WINAPI
#  endif
#endif

namespace e2d { namespace impl
{
    read_file_uptr make_read_file(str_view path) noexcept;
    write_file_uptr make_write_file(str_view path, bool append) noexcept;
}}

namespace e2d { namespace filesystem { namespace impl
{
    bool remove_file(str_view path);
    bool remove_directory(str_view path);

    bool file_exists(str_view path);
    bool directory_exists(str_view path);

    bool create_directory(str_view path);
}}}
