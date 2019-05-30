/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/path.hpp>
#include <enduro2d/utils/strings.hpp>
#include <enduro2d/utils/filesystem.hpp>

#define E2D_FILES_MODE_POSIX  1
#define E2D_FILES_MODE_WINAPI 2

#ifndef E2D_FILES_MODE
#  if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_IOS
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_WINDOWS
#    define E2D_FILES_MODE E2D_FILES_MODE_WINAPI
#  elif defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_BSD
#    define E2D_FILES_MODE E2D_FILES_MODE_POSIX
#  endif
#endif

#ifndef E2D_FILES_MODE
#  error E2D_FILES_MODE not detected
#endif

namespace e2d { namespace impl
{
    read_file_uptr make_read_file(str_view path) noexcept;
    write_file_uptr make_write_file(str_view path, bool append) noexcept;
}}
